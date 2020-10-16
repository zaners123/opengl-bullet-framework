#ifndef OPENGL_SPHERE_H
#define OPENGL_SPHERE_H
#include "SimpleNode.h"
/**A Sphere. Centered.
 * */
class Sphere : public SimpleNode {
	int subdivisions;
	Point pt(double x, double y) {
		return point(
				glm::sin((y / subdivisions) * (2 * M_PI)) * glm::cos(x * 2 * M_PI / subdivisions),
				glm::sin((y / subdivisions) * (2 * M_PI)) * glm::sin(x * 2 * M_PI / subdivisions),
				glm::cos((y / subdivisions) * (2 * M_PI)),
				x,
				y
		);
	}
	void genSphere() {
		for (int r=0;r<subdivisions;r++) {
			for (int i=0;i<subdivisions;i++) {
				addTri(	pt(i,r),
						pt(i,r+1),
						pt(i+1,r));
				addTri( pt(i,r+1),
						pt(i+1,r),
						pt(i+1,r+1));
			}
		}
	}
public:
	Sphere(int subdivisions = 75) : SimpleNode() {
		this->subdivisions = subdivisions;
		genSphere();
		fillBuffers();
		replaceRigidBody();
	}
};
#endif