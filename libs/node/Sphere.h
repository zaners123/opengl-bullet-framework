#ifndef OPENGL_SPHERE_H
#define OPENGL_SPHERE_H
#include "SimpleNode.h"
/**A Sphere. Centered.
 * */
class Sphere : public SimpleNode {
	int subdivisions;
	Point pt(double a, double b, double c) {
		double sd = subdivisions;
		return point(
				glm::sin((c/subdivisions) * (2 * M_PI)) * glm::cos(a * 2 * M_PI / sd),
				glm::sin((c/subdivisions) * (2 * M_PI)) * glm::sin(b * 2 * M_PI / sd),
				glm::cos((c/subdivisions) * (2 * M_PI))
		);
	}
	void genSphere() {
		for (int r=0;r<subdivisions;r++) {
			for (int i=0;i<subdivisions;i++) {
				addTri(	pt(i,i,r),
						pt(i,i,r+1),
						pt(i+1,i+1,r));
				addTri( pt(i,i,r+1),
						pt(i+1,i+1,r),
						pt(i+1,i+1,r+1));
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