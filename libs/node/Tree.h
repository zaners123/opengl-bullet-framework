#ifndef OPENGL_TREE_H
#define OPENGL_TREE_H
#include "NodeVector.h"
#include "Ring.h"
#include "Circle.h"
#include "Cylinder.h"

class Tree : public SimpleNode {

	Point base,rot;
	double r,h;

	int subdivisions;
	void addFractalTri(Point a, Point b, Point c) {

		a.x=r*a.x;a.y=r*a.y;a.z=h*a.z;
		b.x=r*b.x;b.y=r*b.y;b.z=h*b.z;
		c.x=r*c.x;c.y=r*c.y;c.z=h*c.z;

		a=a.rotate(rot.x,rot.y,rot.z);
		b=b.rotate(rot.x,rot.y,rot.z);
		c=c.rotate(rot.x,rot.y,rot.z);

		a.x=base.x+a.x;a.y=base.y+a.y;a.z=base.z+a.z;
		b.x=base.x+b.x;b.y=base.y+b.y;b.z=base.z+b.z;
		c.x=base.x+c.x;c.y=base.y+c.y;c.z=base.z+c.z;

		addTri(a,b,c);
	}
	static Point pt(double a, double b, double c, double sd) {
		return point(
				 glm::cos(a * 2 * M_PI / sd),
				 glm::sin(b * 2 * M_PI / sd),
				 c,glm::cos(b*M_PI/2 / sd ),c
		);
	}
	void cylinder() {
		//bottom/top
		for (int side=0;side<=1;side++) {
			for (int i=0;i<subdivisions;i++) {
				addFractalTri(
						point(0, 0, side,0,0),
						point(glm::cos(2 * M_PI * i / subdivisions), glm::sin(2 * M_PI * i / subdivisions), side,
						      glm::cos(2 * M_PI * i / subdivisions),glm::sin(2 * M_PI * i / subdivisions)),
						point(glm::cos(2 * M_PI * (i + 1) / subdivisions), glm::sin(2 * M_PI * (i + 1) / subdivisions), side,
						      glm::cos(2 * M_PI * (i + 1) / subdivisions), glm::sin(2 * M_PI * (i + 1) / subdivisions))
				);
			}
		}
		//ring
		for (int ring=0; ring < 1; ring++) {
			for (int i=0; i < subdivisions; i++) {
				addFractalTri(pt(i, i, ring, subdivisions),
				              pt(i, i, ring + 1, subdivisions),
				              pt(i + 1, i + 1, ring, subdivisions));
				addFractalTri(pt(i, i, ring + 1, subdivisions),
				              pt(i + 1, i + 1, ring, subdivisions),
				              pt(i + 1, i + 1, ring + 1, subdivisions));
			}
		}
//		replaceRigidBody();
	}
	void cylinder(Point base, Point rot, double r, double h) {
		this->base=base;
		this->rot=rot;
		this->r=r;
		this->h=h;
		cylinder();
	}
public:

	GLfloat rand_neghalf_half() {
		return ((GLfloat)(rand()%256-128))/128;
	}

	void fractal(Point base, Point rot, double r, double h, int depth) {
		cylinder(base,rot,r,h);
		if (depth < 0) return;
		base+=point(0,0,h).rotate(rot.x,rot.y,rot.z);

		//shrink lengths as distance grows
		h *= .8f;
		r *= .6f;

		GLfloat turn = M_PI / 4;
		for (int i=0;i<rand()%4+3;i++)
			fractal(
				base,
	        rot + point(
			        turn * rand_neghalf_half(),
			        turn * rand_neghalf_half(),
			        turn * rand_neghalf_half()
	        		),
			    r,h,depth-1);
	}

	explicit Tree(int subdivisions = 25) : SimpleNode() {
		srand(16);
		this->subdivisions = subdivisions;
		base.x=0;base.y=0;base.z=0;
		rot.x=M_PI/2;rot.y=0;rot.z=0;
		r=1;h=25;
		fractal(base,rot,r,h,3);
		fillBuffers();
	}
};
#endif