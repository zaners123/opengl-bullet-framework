#ifndef OPENGL_NODEBUILDER_H
#define OPENGL_NODEBUILDER_H

#include <cstring>
#include "SimpleNode.h"
#include "Circle.h"
#include "Sphere.h"
#include "Cube.h"
#include "Cylinder.h"

/**
 * TODO plan is for all nodes to be constructed in here
 *
 * todo what to do about nodes with custom construction-time parameters like importing OBJs/STLs?
 *      You could extend NodeBuilder with something like StlBuilder
 * */
class NodeBuilder {
public:
	enum Shape {
		undefined,
		circle,
		cube,
		cylinder,
		sphere,
		quarterBowl,
	};

	//building data
	Shape shape = undefined;
	char* texture = nullptr;
	btScalar mass = 1;

	NodeBuilder() {

	}

	static NodeBuilder start() {
		return NodeBuilder();
	}

	SimpleNode* build() const {
		SimpleNode* ret;
		switch(shape) {
			case undefined:return nullptr;
			case circle:ret = new Circle();break;
			case cube:ret = new Cube();break;
			case cylinder:ret = new Cylinder();break;
			case sphere:ret = new Sphere();break;
			case quarterBowl:ret=new QuarterBowl();break;
		}
		ret->setTexture(texture);
		ret->setMass(mass);
		ret->build();
		return ret;
	}

	NodeBuilder* setMass(btScalar mass) {
		this->mass=mass;
		return this;
	}

	NodeBuilder* setFixed() {
		this->mass=0;
		return this;
	}

	NodeBuilder* setShape(Shape shape) {
		this->shape=shape;
		return this;
	}

	NodeBuilder* setTexture(const char* texture) {
		this->texture = new char[std::strlen(texture)+1];
		std::strcpy(this->texture,texture);
		return this;
	}
};
#endif