#ifndef OPENGL_CONE_H
#define OPENGL_CONE_H
#include "NodeVector.h"
#include "Ring.h"
#include "Circle.h"
class Cone : public SimpleNode {
public:
	static Point pt(double a, double b, double c, double sd) {
		return point(
				glm::cos(a * 2 * M_PI / sd) * (1-c),
				glm::sin(b * 2 * M_PI / sd) * (1-c),
				c,c,glm::sin(b * 2 * M_PI/sd)
		);
	}
private:
	void genRing(int subdivisions) {
		for (int r=0; r < 1; r++) {
			for (int i=0; i < subdivisions; i++) {
				addTri(pt(i, i, r, subdivisions),
				       pt(i, i, r + 1, subdivisions),
				       pt(i + 1, i + 1, r, subdivisions));
				addTri(pt(i, i, r + 1, subdivisions),
				       pt(i + 1, i + 1, r, subdivisions),
				       pt(i + 1, i + 1, r + 1, subdivisions));
			}
		}
	}
public:
	explicit Cone(int subdivisions = 75, bool drawBottom = true) : SimpleNode() {
		//bottom
		if (drawBottom) {
			for (int i=0;i<subdivisions;i++) {
				addTri(
						0, 0, 0,
						glm::cos(2 * M_PI * i / subdivisions), glm::sin(2 * M_PI * i / subdivisions), 0,
						glm::cos(2 * M_PI * (i + 1) / subdivisions), glm::sin(2 * M_PI * (i + 1) / subdivisions), 0
						);
			}
		}
		//ring
		genRing(subdivisions);

		fillBuffers();
	}
};
#endif