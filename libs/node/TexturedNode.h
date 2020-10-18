#ifndef OPENGL_TEXTUREDNODE_H
#define OPENGL_TEXTUREDNODE_H

//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <cstdio>
#include <stack>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Node.h"
#include "../../libs/camera/camera.h"
#include "../commonGlLib.h"
#include "SimpleNode.h"

/**
 * A node, but with all the triangle buffer logic taken care of
 * */
/*class TexturedNode : public SimpleNode {

	unsigned long getNumVerts() {
		return triangleData.size()*3;
	}

	void initBuffers() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
	}
public:
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

	static Point point(GLfloat x, GLfloat y, GLfloat z) {
		Point p;
		p.x = x;
		p.y = y;
		p.z = z;
		return p;
	}

	virtual void replaceRigidBody() override {
		//physics
		std::vector<btVector3> convexPoints;
		for(Triangle& t : triangleData) {
			glm::vec3 a = applyPosOnly(glm::vec3(t.pointA.x,t.pointA.y,t.pointA.z));
			convexPoints.push_back(btVector3(a.x,a.y,a.z));
			glm::vec3 b = applyPosOnly(glm::vec3(t.pointB.x,t.pointB.y,t.pointB.z));
			convexPoints.push_back(btVector3(b.x,b.y,b.z));
			glm::vec3 c = applyPosOnly(glm::vec3(t.pointC.x,t.pointC.y,t.pointC.z));
			convexPoints.push_back(btVector3(c.x,c.y,c.z));
		}
		btConvexHullShape* shape = new btConvexHullShape(&convexPoints[0].getX(), convexPoints.size(), sizeof(btVector3));
		shape->optimizeConvexHull();
		shape->recalcLocalAabb();

		btDefaultMotionState* ms = new btDefaultMotionState();
		btVector3 inertia(0,0,0);
		if (mass!=0) shape->calculateLocalInertia(mass, inertia);
		setRigidBody(new btRigidBody(mass, ms, shape, inertia));
		rb->setFriction(0.1f);
		rb->setRollingFriction(0.1f);
		rb->setSpinningFriction(0.1f);
//		rb->setAnisotropicFriction();
//		rb->setContactStiffnessAndDamping(0.5f,0.5f);
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
		prog = loadShader("../libs/camera/SimpleTextured.vs", "../libs/camera/SimpleTextured.fs");
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

	const GLuint planeTexID = loadTexture("../a7/grass.png");

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(prog);
		glUniformMatrix4fv(glGetUniformLocation(prog, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
		glBindTexture(GL_TEXTURE_2D, planeTexID);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, getNumVerts());
	}
};*/
#endif