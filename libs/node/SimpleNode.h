#ifndef OPENGL_SIMPLENODE_H
#define OPENGL_SIMPLENODE_H

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
#include "Node.h"
#include "../../libs/camera/camera.h"
#include "../commonGlLib.h"

/**
 * A node, but with all the triangle buffer logic taken care of
 * */
class SimpleNode : public Node {
public:
	class Point {
	public:
		GLfloat x,y,z;
		GLfloat texX = 0, texY = 0;
		Point rotate(GLfloat rx, GLfloat ry, GLfloat rz) {
			glm::mat4 rot = glm::mat4(1);
			rot = glm::rotate(rot, rx,glm::vec3(1,0,0));
			rot = glm::rotate(rot, ry,glm::vec3(0,1,0));
			rot = glm::rotate(rot, rz,glm::vec3(0,0,1));

			glm::vec4 tmp = glm::vec4(x,y,z,1);
			Point ret;
			tmp = tmp * rot;
			ret.x=tmp.x;
			ret.y=tmp.y;
			ret.z=tmp.z;
			ret.texX = texX;
			ret.texY = texY;
			return ret;
		}
		Point& operator += (const Point &that) {
			x += that.x;
			y += that.y;
			z += that.z;
			return *this;
		}
		Point operator + (const Point &that) {
			Point ret;
			ret.x = this->x + that.x;
			ret.y = this->y + that.y;
			ret.z = this->z + that.z;
			ret.texX = this->texX;
			ret.texY = this->texY;
			return ret;
		}
	};
	struct Triangle {
		Point pointA;
		Point pointB;
		Point pointC;
	};
private:
	friend class NodeBuilder;
	bool textured;
	GLuint imageTexture = 0;
	std::vector<Triangle> triangleData;
	std::vector<glm::vec3> instanceData;

	int getNumVerts() {
		return triangleData.size()*3;
	}
	void initBuffers() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
	}
public:

	static Point point(GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY) {
		Point p;
		p.x = x;
		p.y = y;
		p.z = z;
		p.texX = texX;
		p.texY = texY;
		return p;
	}
	static Point point(GLfloat x, GLfloat y, GLfloat z) {
		Point p;
		p.x = x;
		p.y = y;
		p.z = z;
		p.texX = 0;
		p.texY = 0;
		return p;
	}
	virtual void replaceRigidBody() override {
		//physics
		std::vector<btVector3> convexPoints;

		int numTris = triangleData.size();
		bool hugeMesh = numTris > 1024;

		int divide = hugeMesh ? 3 : -1;
		int i=0;
		for(Triangle& t : triangleData) {
			if (++i < divide) continue;
			i=0;
			glm::vec3 a = applyPosOnly(glm::vec3(t.pointA.x,t.pointA.y,t.pointA.z));
			convexPoints.push_back(btVector3(a.x,a.y,a.z));
			if (!hugeMesh) {
				glm::vec3 b = applyPosOnly(glm::vec3(t.pointB.x,t.pointB.y,t.pointB.z));
				convexPoints.push_back(btVector3(b.x,b.y,b.z));
				glm::vec3 c = applyPosOnly(glm::vec3(t.pointC.x,t.pointC.y,t.pointC.z));
				convexPoints.push_back(btVector3(c.x,c.y,c.z));
			}
		}
		btConvexHullShape* shape = new btConvexHullShape(&convexPoints[0].getX(), convexPoints.size(), sizeof(btVector3));
		shape->optimizeConvexHull();
		shape->recalcLocalAabb();

		btDefaultMotionState* ms = new btDefaultMotionState();
		btVector3 inertia(0,0,0);
		if (mass!=0) shape->calculateLocalInertia(mass, inertia);
		setRigidBody(new btRigidBody(mass, ms, shape, inertia));
		rb->setFriction(0.3f);
		rb->setRollingFriction(0.4f);
		rb->setSpinningFriction(0.4f);
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

	void addTri(
			GLfloat ax, GLfloat ay, GLfloat az, GLfloat atx, GLfloat aty,
			GLfloat bx, GLfloat by, GLfloat bz, GLfloat btx, GLfloat bty,
			GLfloat cx, GLfloat cy, GLfloat cz, GLfloat ctx, GLfloat cty) {
		Triangle tri;
		tri.pointA.x=ax;tri.pointA.y=ay;tri.pointA.z=az;tri.pointA.texX=atx;tri.pointA.texY=aty;
		tri.pointB.x=bx;tri.pointB.y=by;tri.pointB.z=bz;tri.pointB.texX=btx;tri.pointB.texY=bty;
		tri.pointC.x=cx;tri.pointC.y=cy;tri.pointC.z=cz;tri.pointC.texX=ctx;tri.pointC.texY=cty;
		triangleData.push_back(tri);
	}

	SimpleNode() {
		setTexture(nullptr);
		initBuffers();
		clearTris();
	}

	bool getIsInstanced() {
		return !instanceData.empty();
	}

	/**Sets shader (based on state of isTextured() and getIsInstanced()
	 * @see SimpleNode::isTextured()
	 * @see SimpleNode::getIsInstanced()
	 * */
	void updateShader() {
		if (progLock) return;
		//todo test if prog already set? This might delete other progs?
		glDeleteProgram(prog);
		if (getIsInstanced()) {
			if (isTextured()) {
				prog = loadShader("libs/node/shader/InstancedTextured.vs", "libs/node/shader/InstancedTextured.fs");
			} else {
				prog = loadShader("libs/node/shader/InstancedColored.vs", "libs/node/shader/InstancedColored.fs");
			}
		} else {
			if (isTextured()) {
				prog = loadShader("libs/node/shader/SimpleTextured.vs", "libs/node/shader/SimpleTextured.fs");
			} else {
				prog = loadShader("libs/node/shader/SimpleColored.vs", "libs/node/shader/SimpleColored.fs");
			}
		}
	}

	void setExternalProg(GLuint newProg) {
		prog = newProg;
		progLock = true;
	}

	void setIsInstanced(bool isInstanced) {
		if (isInstanced) {
			setFixed();
		} else {
			instanceData.clear();
		}
		updateShader();
	}

	bool isTextured() {
		return textured;
	}

	void setTexture(const char* texture) {
		textured = texture?true:false;
		updateShader();
		if (texture) {
			glUniform1i(glGetUniformLocation(prog, "Tex1"), 0);
			glActiveTexture(GL_TEXTURE0);
			imageTexture = loadTexture(texture);
		}
	}

	void clearTris() {
		triangleData.clear();
		fillBuffers();
	}

	/**
	 * Call after adding new triangles / instances
	 * */
	void fillBuffers() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER,
			   instanceData.size() * sizeof(glm::vec3) + triangleData.size() * sizeof(Triangle),
			   &triangleData[0], GL_STATIC_DRAW);

		GLsizei stride = 5*sizeof(GLfloat);

		void* offset = 0;

		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(0);  // Position data

//		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, stride, offset);
//		glEnableVertexAttribArray(1); // Color data

		//texture offset (texture is weaved with color/position data)
		offset = (GLfloat*)offset + 3;

		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(2); // Texture Data

		if (getIsInstanced()) {
			offset = (Triangle *) (triangleData.size()* sizeof(Triangle));
			//main instance storage
			glBufferSubData(GL_ARRAY_BUFFER, (unsigned long) offset, sizeof(glm::vec3) * instanceData.size(), &instanceData[0]);
			glVertexAttribPointer(3, 3, GL_FLOAT/*?*/, GL_FALSE, 0, (GLvoid *) offset);
			glEnableVertexAttribArray(3);//instance data
			glVertexAttribDivisor(3, 1);// new position every time
		}

		glBindVertexArray(0);
	}

	void drawDrawable(glm::mat4 wvp) override {
		glUseProgram(prog);
		glUniformMatrix4fv(glGetUniformLocation(prog, "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
		if (textured) {
			glBindTexture(GL_TEXTURE_2D, imageTexture);
		}
		glBindVertexArray(vao);
		if (getIsInstanced()) {
			glDrawArraysInstanced(GL_TRIANGLES, 0, getNumVerts(), instanceData.size());
		} else {
			glDrawArrays(GL_TRIANGLES, 0, getNumVerts());
		}
	}

	/**
	 * Turns this SimpleNode into a fixed, instanced, node. Replaces program with instanced one
	 * */
	void addInstance(glm::vec3 loc) {
		/*todo why is this necessary?*/
		loc*=2;

		instanceData.push_back(loc);
		if (instanceData.size()==1) setIsInstanced(true);
	}

};
#endif