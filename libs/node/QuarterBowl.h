#ifndef OPENGL_QUARTER_BOWL_H
#define OPENGL_QUARTER_BOWL_H
#include "SimpleNode.h"


class QuarterBowl : public SimpleNode {
	Point pt(const int subdivisions, double x, double y) {
		//x wraps, y climbs
		return point(
				glm::sin((y / subdivisions) * (2 * M_PI)) * glm::cos(x * 2 * M_PI / subdivisions),
				-glm::sin((y / subdivisions) * (2 * M_PI)) * glm::sin(x * 2 * M_PI / subdivisions),
				glm::cos((y / subdivisions) * (2 * M_PI)),
//				(glm::abs(1-2*x/subdivisions)) + (2*x/subdivisions<1?1:0),
				x/subdivisions,
				y/subdivisions*4
		);
	}
	void gen(const int subdivisions) {
		for (int y=0;y<=subdivisions/4;y++) {
			for (int x=0;x<=subdivisions/4;x++) {
				addTri(	pt(subdivisions, x, y),pt(subdivisions, x, y + 1),pt(subdivisions, x + 1, y));
				addTri( pt(subdivisions, x, y + 1),pt(subdivisions, x + 1, y),pt(subdivisions, x + 1, y + 1));
			}
		}
	}
public:
	QuarterBowl(int subdivisions = 74) : SimpleNode() {
		gen(subdivisions);
		fillBuffers();
		replaceRigidBody();
	}
};
#endif