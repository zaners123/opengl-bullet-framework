//
// Created by zaners123 on 5/5/20.
//

#ifndef OPENGL_PHYSICSOBJECT_H
#define OPENGL_PHYSICSOBJECT_H

#include "../camera/camera.h"

class PhysicsObject : public virtual Collidable, public virtual Location {

public:
	glm::vec3 acceleration;
	glm::vec3 velocity;


	void physicsLoop() {
		location += velocity;
		velocity += acceleration;
		velocity *= .95f;//drag or something
		acceleration *= .95f;//drag or something
		acceleration += glm::vec3(0,-.01f,0);
	}
};


#endif //OPENGL_PHYSICSOBJECT_H
