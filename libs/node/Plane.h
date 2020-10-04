#ifndef OPENGL_PLANE
#define OPENGL_PLANE

#include "../../libs/camera/camera.h"

#define COLLISION_THICKNESS 0.1f

class Plane : public Node {

	constexpr static GLint numVerts = 6;
	constexpr static GLint vertsAttrNum = 3;
	constexpr static GLint colorAttrNum = 4;

	bool centered;
public:

/*	virtual BoundingBox getBoundingBoxCollidable() override {
		if (centered) {
			return BoundingBox(glm::vec3(-.5f,-.5f,-COLLISION_THICKNESS), glm::vec3(.5f, .5f, COLLISION_THICKNESS));
		} else {
			return BoundingBox(glm::vec3(0,0,-COLLISION_THICKNESS), glm::vec3(1, 1, COLLISION_THICKNESS));
		}
	}*/

	Plane(bool centered) {
		this->centered = centered;

		float s = centered ? .5f : 1.0f;
		float n = centered ? -.5f : 0.0f;


		GLfloat vertices[numVerts*vertsAttrNum] = {
				n,n,0,
				s,n,0,
				s,s,0,
				n,n,0,
				s,s,0,
				n,s,0,
		};

		prog = initShaders("../libs/camera/simple.vs", "../libs/camera/simple.fs");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//allocates the GPU buffer space and fills it
		glBufferData(GL_ARRAY_BUFFER, numVerts*(colorAttrNum+vertsAttrNum)*sizeof(GLfloat), 0, GL_STATIC_DRAW);

		//main vertex storage
		glBufferSubData(GL_ARRAY_BUFFER,0, numVerts*vertsAttrNum*sizeof(GLfloat), vertices);
		glVertexAttribPointer(0,/*attribute number zero*/
		                      vertsAttrNum,/*num traits (XYZ)*/
		                      GL_FLOAT,/*attribute traits are floats*/
		                      GL_FALSE,/*is vector data already normalized? No*/
		                      0,/*stride (like in CNN), data between each item*/
		                      (GLvoid *)0/*first item offset*/
		);
		glEnableVertexAttribArray(0);
		setColor(glm::vec4(1,0,1,0.5f));
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
		glDrawArrays(GL_TRIANGLES, 0,6);
	}

	void setColor(glm::vec4 color) {
		GLulong offset = numVerts*vertsAttrNum*sizeof(GLfloat);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//main color storage
		for (int i=0;i<numVerts;i++) {
			glBufferSubData(GL_ARRAY_BUFFER,offset, sizeof(GLfloat)*colorAttrNum, &color);
			offset += sizeof(GLfloat)*(colorAttrNum);
		}
		glVertexAttribPointer(1,//attribute number one
		                      colorAttrNum,//num traits (rgba)
		                      GL_FLOAT,//attribute traits are floats
		                      GL_FALSE,//is vector data already normalized? No
		                      0,(GLvoid *)(numVerts*vertsAttrNum*sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
};

#endif