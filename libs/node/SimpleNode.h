#ifndef OPENGL_SIMPLENODE_H
#define OPENGL_SIMPLENODE_H

//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <cstdio>
#include <stack>
#include "Node.h"
#include "../../libs/camera/camera.h"

/**
 * A node, but with all the triangle buffer logic taken care of
 * */
class SimpleNode : public Node {

	int getNumVerts() {
		return triangleData.size()*3;
	}

	void initBuffers() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
	}
public:
	struct Color {
		GLfloat x,y,z;
	};
	struct Point {
		GLfloat x,y,z;
	};
	struct Triangle {
		Point pointA;
		Point pointB;
		Point pointC;
	};
private:
	std::vector<Triangle> triangleData;
public:
	bool isCollision(glm::vec3 point) {
		return false;
	}

	static Point point(GLfloat x, GLfloat y, GLfloat z) {
		Point p;
		p.x = x;
		p.y = y;
		p.z = z;
		return p;
	}

	void addTri(Point a, Point b, Point c) {
		Triangle tri;
		tri.pointA = a;
		tri.pointB = b;
		tri.pointC = c;
		triangleData.push_back(tri);
	}

	void addTri(GLfloat ax, GLfloat ay, GLfloat az, GLfloat bx, GLfloat by, GLfloat bz, GLfloat cx, GLfloat cy, GLfloat cz) {
		Triangle tri;
		tri.pointA.x=ax;tri.pointA.y=ay;tri.pointA.z=az;
		tri.pointB.x=bx;tri.pointB.y=by;tri.pointB.z=bz;
		tri.pointC.x=cx;tri.pointC.y=cy;tri.pointC.z=cz;
		triangleData.push_back(tri);
	}


	SimpleNode() {
		prog = initShaders("../libs/camera/simple.vs", "../libs/camera/simple.fs");
		initBuffers();
		clearTris();
	}

	void clearTris() {
		triangleData.clear();
		fillBuffers();
	}

	void fillBuffers() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, triangleData.size() * sizeof(Triangle), &triangleData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);  // Position data

		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1); // Color data

		glBindVertexArray(0);
	}

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(prog);
		glUniformMatrix4fv(glGetUniformLocation(prog, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, getNumVerts());
	}
};
#endif