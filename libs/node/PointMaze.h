#ifndef OPENGL_POINTMAZE
#define OPENGL_POINTMAZE

//magic OpenGL stuff
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <array>

#include "../../libs/camera/camera.h"
#include "../poisson_disk_sampling/poisson_disk_sampling.h"

class PointMaze : public Node {

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

	GLuint simpleProg;

public:

	PointMaze() : PointMaze(10.0f, 100) {

	}

	PointMaze(float kRadius, float size) {

		simpleProg = initShaders("../libs/camera/simple.vs", "../libs/camera/simple.fs");

		points = disks(kRadius, size);
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

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(simpleProg);
		glUniformMatrix4fv(glGetUniformLocation(simpleProg, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//todo draw as triangles? Gotta find a maze generation algo on where to put triangles (graph theory)
//		glDrawElements(GL_POINTS, points.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_PATCHES, 0, points.size());
	}

	bool isCollision(glm::vec3 point) {
		return false;//this is also points so collisions are "unlikely/impossible" in a way
	}
};
#endif
