#ifndef OPENGL_PLANE
#define OPENGL_PLANE

#include "Node.h"
#include "SimpleNode.h"

class Plane : public SimpleNode {
	constexpr static float l = -.5f;
	constexpr static float h = .5f;
	constexpr static float y = 0.f;
	void gen() {
		//yz face low x

		addTri(l,y,l,0,0,l,y,h,0,1,h,y,h,1,1);//013
		addTri(l,y,l,0,0,h,y,h,1,1,h,y,l,1,0);//032
	}
public:
	Plane() : SimpleNode() {
		gen();
		fillBuffers();
//		replaceRigidBody();
	}
};

#endif