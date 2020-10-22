#ifndef OPENGL_SPHERE_H
#define OPENGL_SPHERE_H
#include "SimpleNode.h"
/**A Sphere. Centered. */
class Sphere : public SimpleNode {
	Point pt(int sd, double x, double y) {
		//x wraps, y climbs
		return point(
				glm::sin((y / sd) * (2 * M_PI)) * glm::cos(x * 2 * M_PI / sd),
				glm::sin((y / sd) * (2 * M_PI)) * glm::sin(x * 2 * M_PI / sd),
				glm::cos((y / sd) * (2 * M_PI)),
				x / sd,
				y / sd * 2
		);
	}
public:
	/**
	 * @param sd Number of subdivisions. Total vertices equal to 3*(sd^2)
	 * */
	Sphere(int sd = 54) : SimpleNode() {
		for (int y=0; y <= sd / 2; y++) {
			for (int x=0; x < sd; x++) {
				addTri(pt(sd, x, y), pt(sd, x, y + 1), pt(sd, x + 1, y));
				addTri(pt(sd, x, y + 1), pt(sd, x + 1, y), pt(sd, x + 1, y + 1));
			}
		}
		fillBuffers();
//		replaceRigidBody();
	}
};
#endif