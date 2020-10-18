#ifndef OPENGL_SIMPLE_CUBE_H
#define OPENGL_SIMPLE_CUBE_H
#include "SimpleNode.h"
class Cube : public SimpleNode {
	constexpr static float l = -.5f;
	constexpr static float h = .5f;
	void gen() {
		//yz face low x
		addTri(l,l,l,0,0,l,l,h,0,1,l,h,h,1,1);//013
		addTri(l,l,l,0,0,l,h,h,1,1,l,h,l,1,0);//032
		//yz face high x
		addTri(h,l,l,0,0,h,h,h,1,1,h,l,h,0,1);//475
		addTri(h,l,l,0,0,h,h,l,1,0,h,h,h,1,1);//467
		//xy face low z
		addTri(l,l,l,0,0,h,l,l,1,0,h,h,l,1,1);//046
		addTri(l,l,l,0,0,h,h,l,1,1,l,h,l,0,1);//064
		//xy face high z
		addTri(l,l,h,0,0,h,l,h,1,0,h,h,h,1,1);//175
		addTri(l,l,h,0,0,h,h,h,1,1,l,h,h,0,1);//137
		//xz face low y
		addTri(l,l,l,0,0,h,l,h,1,1,h,l,l,1,0);//054
		addTri(l,l,l,0,0,l,l,h,0,1,h,l,h,1,1);//015
		//xz face high y
		addTri(l,h,l,0,0,h,h,l,1,0,h,h,h,1,1);//267
		addTri(l,h,l,0,0,h,h,h,1,1,l,h,h,0,1);//273
	}
public:
	Cube() : SimpleNode() {
		gen();
		fillBuffers();
		replaceRigidBody();
	}
};
#endif