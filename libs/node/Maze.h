#ifndef OPENGL_MAZE
#define OPENGL_MAZE



//magic OpenGL stuff
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C++ Stuff
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
//my stuff
#include "../../libs/camera/camera.h"
#include "Node.h"
#include "../commonGlLib.h"

class Loc {
public:
	int x,y,z;
	Loc(int x, int y, int z) {
		this->x=x;this->y=y;this->z=z;
	}
	bool operator==(const Loc& rhs) const
	{
		return x==rhs.x && y==rhs.y && z==rhs.z;
	}
};

const static Loc unitVectors[] = {
		Loc(-1,0,0),
		Loc(0,-1,0),
		Loc(0,0,-1),
		Loc(1,0,0),
		Loc(0,1,0),
		Loc(0,0,1),
};

class Maze : public Node {

	int cubeScale = 32;
	float s = float(cubeScale);
	unsigned long numCubes;
	const int vecX = 0;
	const int vecY = 1;
	const int vecZ = 2;
	GLint* instance_positions;
	int  instance_positions_size;
public:
	explicit Maze(int dim) {

		prog = loadShader("camera/instance.vs", "camera/instance.fs");

		if (dim%2==0) {
			std::cerr<<"DO ODD DIM"<<std::endl;
			exit(2);
		}

		GLfloat vertices[] = {
				0,0,0,
				s,0,0,
				0,s,0,
				s,s,0,
				0,0,s,
				s,0,s,
				0,s,s,
				s,s,s,
		};
		GLfloat vertexColor[] = {
				0,0,0,0,
				1,0,0,0,
				0,1,0,1,
				1,1,0,1,
				0,0,1,1,
				1,0,1,1,
				0,1,1,1,
				1,1,1,1,
		};
		GLint indices[] = {
				0,1,3,3,2,0,//botton
				5,4,6,6,7,5,//top
				4,0,1,1,5,4,//back
				2,6,7,7,3,2,//front
				4,6,2,2,0,4,
				1,5,7,7,3,1,
		};


		bool* mazeBools = generateMazeStructure(dim);
		//count the cubes we need to see how much memory to allocate and such
		numCubes = 0;
		for(int i=0;i<dim*dim*dim;i++) if (mazeBools[i]) numCubes++;
		const unsigned long vecSize = 3;
		GLint* maze;
		instance_positions_size = vecSize * numCubes * sizeof(GLint);
		maze = (GLint*)malloc(instance_positions_size);
		int cubeID = 0;
		for(int x=0;x<dim;x++) {
			for(int y=0;y<dim;y++) {
				for (int z = 0; z < dim; z++) {
					int id = x*dim*dim + y*dim + z;
					bool set = *(mazeBools + sizeof(bool)*id);
					if (!set) continue;
//				std::cout<<x<<','<<y<<','<<z<<'='<<set<<std::endl;
					GLint *c = &maze[cubeID * vecSize];
					c[vecX] = cubeScale * x;
					c[vecY] = cubeScale * y;
					c[vecZ] = cubeScale * z;
					cubeID++;
				}
			}
		}
		instance_positions = maze;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//allocates the GPU buffer space and fills it
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(vertexColor) + instance_positions_size, 0, GL_STATIC_DRAW);
		GLulong offset = 0;

		//main vertex storage
		glBufferSubData(GL_ARRAY_BUFFER,offset, sizeof(vertices), vertices);
		glVertexAttribPointer(0,/*attribute number zero*/
		                      3,/*num traits (XYZ)*/
		                      GL_FLOAT,/*attribute traits are floats*/
		                      GL_FALSE,/*is vector data already normalized? No*/
		                      0,/*stride (like in CNN), data between each item*/
		                      (GLvoid *)offset/*first item offset*/
		);
		glEnableVertexAttribArray(0);
		offset += sizeof(vertices);

		//main color storage
		glBufferSubData(GL_ARRAY_BUFFER,offset, sizeof(vertexColor), vertexColor);
		glVertexAttribPointer(1,//attribute number one
		                      4,//num traits (rgba)
		                      GL_FLOAT,//attribute traits are floats
		                      GL_FALSE,//is vector data already normalized? No
		                      0,(GLvoid *)offset);
		glEnableVertexAttribArray(1);
		offset += sizeof(vertexColor);

		//main instance storage
		glBufferSubData(GL_ARRAY_BUFFER,offset,instance_positions_size,instance_positions);
		glVertexAttribPointer(2,3,GL_INT,GL_FALSE,0,(GLvoid *)offset);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2,1);// new position every time

//	glBufferSubData(GL_ARRAY_BUFFER,offset,instance_positions_size,instance_positions);
//	glVertexAttribPointer(3,3,GL_INT,GL_FALSE,3,(GLvoid *)offset);
//	glEnableVertexAttribArray(2);
//	glVertexAttribDivisor(3,1);// new position every time

		offset += instance_positions_size;

		//say the triangles the verts are connected by
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//set attributes

		//main my color attribute, put after the vert attribute
	}

	static bool* memcoord(bool* ret, int dim, int x, int y, int z) {
		return (ret + sizeof(bool) * (x * dim * dim + y * dim + z));
	}
	static bool* memcoord(bool* ret, int dim, Loc l) {
		return memcoord(ret,dim,l.x,l.y,l.z);
	}

	template <class T> bool contains(std::vector<T>& v, T mightContain) {
//	if (mightContain == nullptr) return false;
		return find(v.begin(), v.end(), mightContain) != v.end();
	}


	bool isSolvable(bool* maze, int dim, Loc start, Loc end) {
		std::stack<Loc> viewStack;
		std::vector<Loc> visited;
		viewStack.push(start);
		do {
			Loc nodeFrom = viewStack.top();
			//don't view twice
			viewStack.pop();
			//already seen it? Continue.
			if (contains(visited, nodeFrom)) continue;
			//Reached the end? Solveable!
			if (nodeFrom.x==end.x &&
			    nodeFrom.y==end.y &&
			    nodeFrom.z==end.z) {
				return true;
			}
			//look at neighbord
			for (auto unit : unitVectors) {
				Loc newLoc = Loc(nodeFrom.x+unit.x,nodeFrom.y+unit.y,nodeFrom.z+unit.z);
				if (newLoc.x<0 || newLoc.x>=dim ||
				    newLoc.y<0 || newLoc.y>=dim ||
				    newLoc.z<0 || newLoc.z>=dim
						) continue;
				//if empty, add to viewStack
				if (!*memcoord(maze,dim,newLoc)) viewStack.push(newLoc);
			}
			//visited
			visited.push_back(nodeFrom);
		} while (!viewStack.empty());
		return false;
	}
/**@param 3d array of dim x dim x dim that will be filled where true means put a cube there
 * */
	bool/*[][][]*/* generateMazeStructure(int dim) {
		srand((int)std::time(nullptr));
		bool* ret = (bool*)malloc(sizeof(bool)*dim*dim*dim);
		//todo an actual maze-generator algorithm
		for(int x=0;x<dim;x++) {
			for(int y=0;y<dim;y++) {
				for (int z = 0; z < dim; z++) {
					bool set = !(x%2==1 && y%2==1 && z%2==1);
					/*bool set =
							//corners
							(x%2==0 && y%2==0 && z%2==0) ||
							//walls
							(x == 0 || y == 0 || z == 0 || x == dim - 1 || y == dim - 1 || z == dim - 1);*/
					*memcoord(ret,dim, x, y, z) = set;
				}
			}
		}
		for(int x=1;x<dim-1;x+=2) {
			for(int y=1;y<dim-1;y+=2) {
				for (int z = 1; z < dim-1; z+=2) {
					//i forget what this gen's called, but its the simplest one

					//todo instead of doing this, have it run once choosing x+1 or y+1 then once choosing x+1 or z+1
					switch (rand()%3) {
						case 0:
							if (x!=1) {
								*memcoord(ret,dim, x-1, y, z) = false;
								break;
							}
						case 1:
							if (y!=1) {
								*memcoord(ret,dim, x, y-1, z) = false;
								break;
							}
						case 2:
							if (z!=1) {
								*memcoord(ret,dim, x, y, z-1) = false;
								break;
							}
						default:
							if (x!=1) {
								*memcoord(ret,dim, x-1, y, z) = false;
								break;
							} else if (y!=1) {
								*memcoord(ret,dim, x, y-1, z) = false;
								break;
							} else if (z!=1) {
								*memcoord(ret,dim, x, y, z-1) = false;
								break;
							}
					}
				}
			}
		}
		*memcoord(ret,dim, 0, 1, 1) = false;	//entrance hole
		*memcoord(ret,dim, dim-2, dim-1, dim-2) = false;	//exit hole
		bool solveable = isSolvable(ret, dim, Loc(0,1,1),Loc(dim-2,dim-1,dim-2));
		if (!solveable) {
			std::cerr<<"MAZE NOT SOLVEABLE"<<std::endl;
			exit(1);
		}
		return ret;
	}

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(prog);
		glUniformMatrix4fv(glGetUniformLocation(prog, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);


		//defines vertices attributes (color, location, etc)
		glBindVertexArray(vao);
		//used to pick what vertices to pick
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//	glDrawElements(drawType, 36, GL_UNSIGNED_INT, 0);
		//based off of instanced square
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, numCubes);
	}
};
#endif //OPENGL_MAZE