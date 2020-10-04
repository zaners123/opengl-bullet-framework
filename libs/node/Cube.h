#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H



//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <cstdio>
#include <stack>
#include "Node.h"

class Cube : public Node {
	bool centered;
public:
	Cube(const bool centered) {
		this->centered = centered;

		//physics
		btBoxShape* boxCollisionShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));
		btScalar boxMass = 1;
		btDefaultMotionState* boxMotionState = new btDefaultMotionState();
		btVector3 localInertia(0, 0, 0);
		boxCollisionShape->calculateLocalInertia(boxMass, localInertia);
		rb = new btRigidBody(boxMass, boxMotionState, boxCollisionShape, localInertia);
		if (!centered) {//centered by default I think
			btTransform rbLoc = rb->getCenterOfMassTransform();
			rbLoc.setOrigin(rbLoc.getOrigin()+btVector3(-.5f,-.5f,-.5f));
			rb->setCenterOfMassTransform(rbLoc);
		}

		//not-physics
		prog = initShaders("../libs/camera/simple.vs", "../libs/camera/simple.fs");
		float away = centered ? .5f : 1.0f;
		float near = centered ? -.5f : 0.0f;
		GLfloat cube_vertices[] = {away, away, away, 1.0f,  // v0,v1,v2,v3 (front)
							        near, away, away, 1.0f,
							       near, near, away, 1.0f,
							       away, near, away, 1.0f,
							       away, away, away, 1.0f,  // v0,v3,v4,v5 (right)
		                            away, near, away, 1.0f,
		                           away, near, near, 1.0f,
		                           away, away, near, 1.0f,
		                           away, away, away, 1.0f,  // v0,v5,v6,v1 (top)
		                            away, away, near, 1.0f,
		                           near, away, near, 1.0f,
		                           near, away, away, 1.0f,
		                           near, away, away, 1.0f, // v1,v6,v7,v2 (left)
		                            near, away, near, 1.0f,
		                           near, near, near, 1.0f,
		                           near, near, away, 1.0f,
		                           near, near, near, 1.0f,// v7,v4,v3,v2 (bottom)
		                            away, near, near, 1.0f,
		                           away, near, away, 1.0f,
		                           near, near, away, 1.0f,
		                           away, near, near, 1.0f,// v4,v7,v6,v5 (back)
		                            near, near, near, 1.0f,
		                           near, away, near, 1.0f,
		                           away, away, near, 1.0f };
		GLfloat cube_colors[] = {
				1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f,
				0.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f,
				1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		};
		GLushort cube_indices[] = { 0, 1, 2, 2, 3, 0,      // front
		                            4, 5, 6, 6, 7, 4,      // right
		                            8, 9, 10, 10, 11, 8,      // top
		                            12, 13, 14, 14, 15, 12,      // left
		                            16, 17, 18, 18, 19, 16,      // bottom
		                            20, 21, 22, 22, 23, 20 };    // back
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int handle[3];
		glGenBuffers(3, handle);

		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);  // Vertex position


		glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(prog);
		glUniformMatrix4fv(glGetUniformLocation(prog, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}

	/*virtual BoundingBox getBoundingBoxCollidable() override {
		if (centered) {
			return BoundingBox(glm::vec3(-.5f,-.5f,-.5f),glm::vec3(.5f,.5f,.5f));
		} else {
			return BoundingBox(glm::vec3(0,0,0),glm::vec3(1,1,1));
		}
	}*/

};
#endif //OPENGL_CUBE_H
