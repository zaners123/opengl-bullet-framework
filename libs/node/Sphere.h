#ifndef OPENGL_SPHERE_H
#define OPENGL_SPHERE_H
#include "SimpleNode.h"
/**A Sphere. Centered.
 * */
class Sphere : public SimpleNode {
	int subdivisions;
	Point pt(double x, double y) {
		//x wraps, y climbs
		return point(
				glm::sin((y / subdivisions) * (2 * M_PI)) * glm::cos(x * 2 * M_PI / subdivisions),
				glm::sin((y / subdivisions) * (2 * M_PI)) * glm::sin(x * 2 * M_PI / subdivisions),
				glm::cos((y / subdivisions) * (2 * M_PI)),
//				(glm::abs(1-2*x/subdivisions)) + (2*x/subdivisions<1?1:0),
				x/subdivisions,
				y/subdivisions*2
		);
	}
	void genSphere() {
		for (int y=0;y<=subdivisions/2;y++) {
			for (int x=0;x<subdivisions;x++) {
				addTri(	pt(x, y),pt(x, y + 1),pt(x + 1, y));
				addTri( pt(x, y + 1),pt(x + 1, y),pt(x + 1, y + 1));
			}
		}
	}
public:
	Sphere(int subdivisions = 54) : SimpleNode() {
		//make sure it's even
		if (!(subdivisions/2)) subdivisions++;

		this->subdivisions = subdivisions;
		genSphere();
		fillBuffers();
		replaceRigidBody();
	}
};
#endif