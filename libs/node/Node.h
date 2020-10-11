#ifndef OPENGL_NODE_H
#define OPENGL_NODE_H

//#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <vector>
#include "Drawable.h"
#include "Collidable.h"
#include "Orientable.h"

/**
 * A Node is:
 *  - Drawable (can call draw() on it given a wvp matrix)
 *  - Orientable (can move/scale it around, so it could have functions like move(5,0,0) or scale(2,2,2).)
 *  - Collidable (Has a btRigidBody that it can base its position off of)
 *
 *  Draw a node by calling draw(wvp)
 *
 * */
class Node : protected Drawable, public Collidable, public Orientable {

public:

	Node() : Orientable() {

	}

	/**Set the Center of Mass. Assumes you're calling something like this:
	 *
	 * setBullet(
	 * 			getRigidBody()->getCenterOfMassPosition(),
	 * 			getRigidBody()->getOrientation()
	 * 	);
	 *
	 * */
	virtual void setBullet(const btVector3& btCom, const btQuaternion& btOrient) {
		this->com = glm::vec3(
				btCom.x(),
				btCom.y(),
				btCom.z()
		);
		this->orientation = glm::toMat4(glm::qua(
				btOrient.w(),
				btOrient.x(),
				btOrient.y(),
				btOrient.z()
		));
	}

	virtual void move(GLfloat x, GLfloat y, GLfloat z) {
//		setPos(glm::translate(getPos(), glm::vec3(x,y,z)));
		std::cout<<"COM CHANGING from"<<com.x<<","<<com.y<<","<<com.z<<std::endl;
		com = glm::translate(glm::mat4(1),glm::vec3(x,y,z)) * glm::vec4(com.x,com.y,com.z,1);
		std::cout<<"COM CHANGING to"<<com.x<<","<<com.y<<","<<com.z<<std::endl;

		btTransform tmp = rb->getCenterOfMassTransform();
		tmp.setOrigin(btVector3(tmp.getOrigin().x()+x,tmp.getOrigin().y()+y,tmp.getOrigin().z()+z));
		rb->setCenterOfMassTransform(tmp);
	}

	virtual void updateUsingRigidBody() override {
		if (!getRigidBody()) return;
		setBullet(
				getRigidBody()->getCenterOfMassPosition(),
				getRigidBody()->getOrientation()
		);
	}

	virtual void setMass(const btScalar mass) {
		this->mass = mass;
		replaceRigidBody();
	}

	virtual void setVelocity() {
		rb->
	}

	/**
	 * Fixes it into the world. Same as setMass(0)
	 * */
	virtual void setFixed() {
		setMass(0);
	}

	/**Call to draw a node. This makes the Drawable/Orientable integration*/
	void const draw(glm::mat4 wvp) {
		drawDrawable(applyPos(wvp));
	}
};

#endif //OPENGL_NODE_H
