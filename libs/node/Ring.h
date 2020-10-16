#ifndef OPENGL_Ring_H
#define OPENGL_Ring_H
#include "SimpleNode.h"
class Ring : public SimpleNode {
	Point pt(double a, double b, double c, double sd) {
		return point(
				glm::cos(a * 2 * M_PI / sd),
				glm::sin(b * 2 * M_PI / sd),
				c / sd,0,0
		);
	}
	void genRing(int sd) {
		for (int r=0; r < sd; r++) {
			for (int i=0; i < sd; i++) {
				addTri(	pt(i, i, r, sd),
						pt(i, i,r+1, sd),
						pt(i+1,i+1, r, sd));
				addTri( pt(i, i,r+1, sd),
						pt(i+1,i+1, r, sd),
						pt(i+1,i+1,r+1, sd));
			}
		}
	}
public:
	Ring(int subdivisions = 75) : SimpleNode() {
		genRing(subdivisions);
		fillBuffers();
	}
};
#endif