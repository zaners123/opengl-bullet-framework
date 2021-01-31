#ifndef OPENGL_GRASS_H
#define OPENGL_GRASS_H
#include "../NodeVector.h"
#include "../Ring.h"
#include "../Circle.h"
class Grass : public SimpleNode {
public:
	explicit Grass(int subdivisions = 75, bool drawBottom = true) : SimpleNode() {
		addTri(-0.3f, 0.0f,0.0f,0.3f, 0.0f,0.0f,-0.20f, 1.0f,0.0f);
		addTri(-0.3f, 0.0f,0.0f,-0.20f, 1.0f,0.0f,0.1f, 1.3f,0.0f);
		addTri(-0.3f, 0.0f,0.0f,0.1f, 1.3f,0.0f,0.1f, 1.3f,0.0f);
		addTri(-0.3f, 0.0f,0.0f,0.1f, 1.3f,0.0f,-0.05f, 2.3f,0.0f);
		addTri(-0.3f, 0.0f,0.0f,0.1f, 1.3f,0.0f,-0.05f, 2.3f,0.0f);
				/*0.3f, 0.0f,0.0f,1.0f,
				-0.20f, 1.0f,0.0f, 1.0f,
				0.1f, 1.3f,0.0f, 1.0f,
				-0.05f, 2.3f,0.0f, 1.0f,
				0.0f, 3.3f, 0.0f, 1.0f,*/



		fillBuffers();
	}
};
#endif