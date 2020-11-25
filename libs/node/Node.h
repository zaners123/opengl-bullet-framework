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

	virtual void setCOM(glm::vec3 newCom) {
		this->com = newCom;
		btTransform tmp = rb->getCenterOfMassTransform();
		tmp.setOrigin(btVector3(newCom.x, newCom.y, newCom.z));
		rb->setCenterOfMassTransform(tmp);
	}
	virtual void setCOM(GLfloat x, GLfloat y, GLfloat z) {
		setCOM(glm::vec3(x,y,z));
	}

	virtual void rotate(GLfloat x, GLfloat y, GLfloat z) {
		//todo
	}

	virtual void move(GLfloat x, GLfloat y, GLfloat z) {
		setCOM(com.x + x,com.y + y,com.z + z);
	}

	virtual void updateUsingRigidBody() override {
		if (!getRigidBody()) return;
		setBullet(
				getRigidBody()->getCenterOfMassPosition(),
				getRigidBody()->getOrientation()
		);
	}

	virtual void setMass(const btScalar mass) {
		if (this->mass == mass) return;
		this->mass = mass;
		replaceRigidBody();
	}

	bool built = false;
	virtual void attemptReplaceRigidBody() {
		if (built) {
			replaceRigidBody();
		}
	}

	void setBuilt(bool built) {
		this->built = built;
	}

	/**
	 * Sets an instantaneous change in velocity. Could also be called a velocity offset.
	 * */
	virtual bool setInstantaneousChangeInVelocity(btScalar x, btScalar y, btScalar z) {
		if (!rb) {
			std::cerr<<"setInstantaneousChangeInVelocity caled on null rigidbody"<<std::endl;
			return false;
		}
		rb->setLinearVelocity(btVector3(x,y,z) - rb->getLinearVelocity());
		return true;
	}

	/**
	 * Sets linear velocity
	 * */
	virtual bool setLinearVelocity(btScalar x, btScalar y, btScalar z) {
		if (!rb) {
			std::cerr<<"setLinearVelocity caled on null rigidbody"<<std::endl;
			return false;
		}
		rb->setLinearVelocity(btVector3(x,y,z));
		return true;
	}

	/**
	 * Sets linear velocity
	 * */
	virtual bool setAngularVelocity(btScalar x, btScalar y, btScalar z) {
		if (!rb) {
			std::cerr<<"setAngularVelocity caled on null rigidbody"<<std::endl;
			return false;
		}
		rb->setAngularVelocity(btVector3(x,y,z));
		return true;
	}

	/**
	 * Fixes it into the world. Same as setMass(0)
	 * */
	virtual void setFixed() {
		setMass(0);
	}

	/**Call to draw a node. This makes the Drawable/Orientable integration*/
	void draw(glm::mat4 wvp) {
		drawDrawable(applyPos(wvp));
	}
};
#endif //OPENGL_NODE_H