#ifndef OPENGL_SIMPLE_CUBE_H
#define OPENGL_SIMPLE_CUBE_H

#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "SimpleNode.h"

class SimpleCube : public SimpleNode {

	void gen() {
		const float l = -.5f;
		const float h = .5f;

		//yz face low x
		addTri(l,l,l,l,l,h,l,h,h);//013
		addTri(l,l,l,l,h,h,l,h,l);//032
		//yz face high x
		addTri(h,l,l,h,h,h,h,l,h);//475
		addTri(h,l,l,h,h,l,h,h,h);//467
		//xy face low z
		addTri(l,l,l,h,l,l,h,h,l);//046
		addTri(l,l,l,h,h,l,l,h,l);//064
		//xy face high z
		addTri(l,l,h,h,l,h,h,h,h);//175
		addTri(l,l,h,h,h,h,l,h,h);//137
		//xz face low y
		addTri(l,l,l,h,l,h,h,l,l);//054
		addTri(l,l,l,l,l,h,h,l,h);//015
		//xz face high y
		addTri(l,h,l,h,h,l,h,h,h);//267
		addTri(l,h,l,h,h,h,l,h,h);//273
	}

public:
	SimpleCube() : SimpleNode() {
		gen();
		fillBuffers();
		replaceRigidBody();
	}

	/*void replaceRigidBody() override {
		btBoxShape* shape = new btBoxShape(btVector3(1,1,1));
		btDefaultMotionState* ms = new btDefaultMotionState();
		btVector3 inertia(0,0,0);
		if (mass!=0) shape->calculateLocalInertia(mass, inertia);
		setRigidBody(new btRigidBody(mass, ms, shape, inertia));
	}*/
};
#endif