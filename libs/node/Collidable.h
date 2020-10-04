#ifndef OPENGL_COLLIDABLE_H
#define OPENGL_COLLIDABLE_H

#include <BulletDynamics/Dynamics/btRigidBody.h>

/**
 * A collidable object has an internal btRigidBody, so it can collide with other objects
 * */
class Collidable {
public:
	btRigidBody* rb;
	/**Returns the corresponding Bullet Rigid Body*/
	virtual btRigidBody* getRigidBody() {
		return rb;
	}
	/**
	 * This should be called whenever the Rigid Body moves, so the corresponding OpenGL object can move with it
	 * */
	virtual void updateUsingRigidBody() {
		return;
	}
};
#endif
