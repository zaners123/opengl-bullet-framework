//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <array>
//my stuff
#include "../camera/camera.h"


const float PI = 3.14159265358979323;

//represents a 3d point, made by integers
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

GLint instanceUniformWVP;

const static Loc unitVectors[] = {
		Loc(-1,0,0),
		Loc(0,-1,0),
		Loc(0,0,-1),
		Loc(1,0,0),
		Loc(0,1,0),
		Loc(0,0,1),
};

/**An object containing maze logic. Initialize it, then draw it! That's all!
 * */
class Maze : Drawable {
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

	bool isCollision(glm::vec3 player) {
		for(int cube=0; cube < numCubes; cube++) {
			GLint* c = &instance_positions[cube * 3];
//			if (isCollidingAABB(player,player,c[vecX],c[vecY],c[vecZ],float(c[vecX])+s,float(c[vecY])+s,float(c[vecZ])+s)) {
//				return true;
//				break;
//			}
		}
		return false;
	}

	void draw(glm::mat4 wvp) override {}
	void draw() override {
		//defines vertices attributes (color, location, etc)
		glBindVertexArray(vao);
		//used to pick what vertices to pick
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//	glDrawElements(drawType, 36, GL_UNSIGNED_INT, 0);
		//based off of instanced square
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, numCubes);
	}
};

class PointMaze : Drawable {

	std::vector<std::array<float, 3>> points;
	unsigned long pointsize;


	std::vector<std::array<float, 3>> colors;
	unsigned long colorsize;


	void genColors() {
		for (int i=0;i<points.size();i++) {
			colors.push_back({float(points.at(i).at(0))/100,float(points.at(i).at(1))/100,float(points.at(i).at(2))/100});
		}
		colorsize = sizeof(colors.at(0))*colors.size();
//		for(int i=0;i<colors.size();i++) {
//			std::cout<<"COLOR"<<i<<" = "<<colors.at(i).at(0)<<colors.at(i).at(1)<<colors.at(i).at(2)<<std::endl;
//		}
	}

public:

	PointMaze() {

		points = disks(10.0f, 100);
		pointsize = sizeof(points.at(0))*points.size();
		std::cout << "POINTS: "<<points.size() << std::endl;

		genColors();

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);


		glBufferData(GL_ARRAY_BUFFER, pointsize+colorsize, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pointsize, &points[0]);
		glBufferSubData(GL_ARRAY_BUFFER, pointsize, colorsize, &colors[0]);

		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)pointsize);
		glEnableVertexAttribArray(1);



		//todo rest of this, also add color or at least make em all random colors, then maybe randomly connect em (replace points with triangles) and see how it looks
	}

//	bool isCollision(glm::vec3 player) override {
//		return false;
//	}

	void draw(glm::mat4 wvp) override {}
	void draw() override {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//todo draw as triangles? Gotta find a maze generation algo on where to put triangles (graph theory)
//		glDrawElements(GL_POINTS, points.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_PATCHES, 0, points.size());
	}
};

Maze* m;
PointMaze* pm;


static Camera* cam;

class Timer {
	using clock = std::chrono::high_resolution_clock;
	std::chrono::time_point<clock> start;
public:
	Timer() : start(clock::now()) {}
	double getElapsed() const {
		return std::chrono::duration_cast<std::chrono::duration<double,std::ratio<1>>>(clock::now() - start).count();
	}
	~Timer() {
		std::cout<<"Time: "<<getElapsed()<<"s\n";
	}
};
GLuint instanceProg;
GLuint simpleProg;
auto drawType = GL_TRIANGLES;
//keys held down

//used to get FPS
/*double lastTime = 0;
int nbFrames = 0;
Timer t;
double printAt = t.getElapsed();*/

void init() {
	simpleProg = initShaders("../camera/simple.vs", "../camera/simple.fs");
	instanceProg = initShaders("../camera/instance.vs", "../camera/instance.fs");
	//background color
	glClearColor(1,1,1,1);
//	glClearColor(0,0,0,1);
	glLineWidth(10);
	glPointSize(5);
	glEnable(GL_DEPTH_TEST);

	m = new Maze(7);
	pm = new PointMaze();
}

void display() {

	/*double currentTime = t.getElapsed();
	nbFrames++;
	if (printAt < currentTime) {
		// printf and reset timer
		printf("%fFPS\n", double(nbFrames)/currentTime);
		printAt = currentTime+1;
	}*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//main update camera using magic camera maths
	auto wvp = cam->getWVP();

//	wvp = glm::rotate(wvp,45.0f,glm::vec3(0,1,1));

	//main draw cube maze
	glUseProgram(instanceProg);
//	glUniformMatrix4fv(glGetUniformLocation(instanceProg, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
	m->draw();

	glUseProgram(simpleProg);
	glUniformMatrix4fv(glGetUniformLocation(simpleProg, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
	pm->draw();


	//draw point mesh

	glFlush();
//	glutSwapBuffers();
//	glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y) {
	if (cam->toggleKeys(key,false)) {
		glutPostRedisplay();
		return;
	}
	glutPostRedisplay();
}

void keydown(unsigned char key, int x, int y) {
	if (cam->toggleKeys(key,true)) {
		glutPostRedisplay();
		return;
	}
	switch(key) {
		case 'T':case't':
			if (drawType == GL_TRIANGLES) {
				drawType = GL_LINE_STRIP;
			} else {
				drawType = GL_TRIANGLES;
			}
			break;
	}
	glutPostRedisplay();
}

void gameLoop(int n) {
	cam->gameLoop();

	glutTimerFunc(20, gameLoop, n);
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	cam = new Camera(2000,1000);
	glutCreateWindow("Square");

	if (glewInit()) {
		printf("%s\n", "Unable to initialize GLEW ...");
		return 1;
	}

	init();
	/* we can now get data for the specific OpenGL instance we created */
	printf("GL Vendor : %s\n", glGetString(GL_RENDERER));
	printf("GL Renderer : %s\n", glGetString(GL_VENDOR));
	printf("GL Version (string) : %s\n", glGetString(GL_VERSION));
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//camera library
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutReshapeFunc(cam->reshape);
	glutPassiveMotionFunc(cam->rotateWorld);
	glutSetCursor(GLUT_CURSOR_NONE);
	//other stuff
	glutDisplayFunc(display);
	glutTimerFunc(100, gameLoop, 0);
	glutFullScreen();//todo fullscreen
	glutMainLoop();
	return 0;
}