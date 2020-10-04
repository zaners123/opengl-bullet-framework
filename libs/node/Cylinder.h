#ifndef OPENGL_CYLINDER_H
#define OPENGL_CYLINDER_H
#include "NodeVector.h"
#include "Ring.h"
#include "Circle.h"
class Cylinder : public NodeVector {
public:
	Cylinder(int subdivisions = 75) : NodeVector() {
		push(new Ring(subdivisions));
		push(new Circle(subdivisions));
		push((new Circle(subdivisions))->getMove(0,0,1));
	}
};
#endif