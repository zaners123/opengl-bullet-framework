#ifndef OPENGL_CIRCLE_H
#define OPENGL_CIRCLE_H
#include "SimpleNode.h"
/**A Circle. Centered. */
class Circle : public SimpleNode {
public:
	Circle(int subdivisions = 75) : SimpleNode() {
		for (int i=0;i<subdivisions;i++) {
			addTri( 0,0,0,
					glm::cos(2*M_PI*i/subdivisions),glm::sin(2*M_PI*i/subdivisions),0,
					glm::cos(2*M_PI*(i+1)/subdivisions),glm::sin(2*M_PI*(i+1)/subdivisions),0
			);
		}
		fillBuffers();
	}
};
#endif