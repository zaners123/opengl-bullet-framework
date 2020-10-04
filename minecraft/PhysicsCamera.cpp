//
// Created by zaners123 on 5/5/20.
//

#include "PhysicsCamera.h"

void PhysicsCamera::moveBy(glm::vec3 move) {
//	location = glm::vec3(0,0,0);
	Camera::moveBy(move);
}

PhysicsCamera::PhysicsCamera(int i, int i1) : Camera(i, i1) {}
