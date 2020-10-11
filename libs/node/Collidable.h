#ifndef OPENGL_COLLIDABLE_H
#define OPENGL_COLLIDABLE_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

/**
 * A collidable object has an internal btRigidBody, so it can collide with other objects
 * */
class Collidable {

public:
	btScalar mass = 1;
	btRigidBody* rb=0;

	virtual void setRigidBody(btRigidBody* rbNew) {
		if (rb) {
			btTransform tmp = rb->getCenterOfMassTransform();
			delete rb;
			rb = rbNew;
			rb->setCenterOfMassTransform(tmp);
		} else {
			rb = rbNew;
			//todo would be more stable if COM was set to applyPos(0,0,0);
//			rb->setCenterOfMassTransform()
		}
	}

	/**Returns the corresponding Bullet Rigid Body*/
	btRigidBody* const getRigidBody() {
		return rb;
	}
	/**
	 * This should be called whenever the Rigid Body moves, so the corresponding OpenGL object can move with it
	 * */
	virtual void updateUsingRigidBody() {
		return;
	}

	virtual void setPhysicsWorld(btDiscreteDynamicsWorld* world) {
		if (!rb) return;
		world->addCollisionObject(rb);
	}
};
#endif
