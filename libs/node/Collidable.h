#ifndef OPENGL_COLLIDABLE_H
#define OPENGL_COLLIDABLE_H

#include <BulletDynamics/Dynamics/btRigidBody.h>

class Collidable {
public:
	btRigidBody* rb;
	/**Returns the corresponding Bullet Rigid Body*/
	virtual btRigidBody* getRigidBody() {
		return rb;
	}
};
#endif
