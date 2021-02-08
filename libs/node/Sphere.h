#ifndef OPENGL_SPHERE_H
#define OPENGL_SPHERE_H
#include "SimpleNode.h"
/**A Sphere. Centered. */
class Sphere : public SimpleNode {
	Point pt(int sd, double x, double y,double scale) {
		//x wraps, y climbs
		return point(
				scale* glm::sin((y / sd) * (2 * M_PI)) * glm::cos(x * 2 * M_PI / sd),
				scale* glm::sin((y / sd) * (2 * M_PI)) * glm::sin(x * 2 * M_PI / sd),
				scale* glm::cos((y / sd) * (2 * M_PI)),
				x / sd,
				y / sd * 2
		);
	}
public:
	/**
	 * @param sd Number of subdivisions. Total vertices equal to 3*(sd^2)
	 * */
	Sphere(int sd = 54, double scale=1) : SimpleNode() {
		for (int y=0; y <= sd / 2; y++) {
			for (int x=0; x < sd; x++) {
				addTri(pt(sd, x, y,scale), pt(sd, x, y + 1,scale), pt(sd, x + 1, y,scale));
				addTri(pt(sd, x, y + 1,scale), pt(sd, x + 1, y,scale), pt(sd, x + 1, y + 1,scale));
			}
		}
		fillBuffers();
//		replaceRigidBody();
	}
};
#endif