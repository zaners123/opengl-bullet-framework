//
// Created by zaners123 on 5/5/20.
//
#include "../camera/camera.h"
#include "PhysicsObject.h"

#ifndef OPENGL_PHYSICSCAMERA_H
#define OPENGL_PHYSICSCAMERA_H

class PhysicsCamera : public Camera, public PhysicsObject {

public:

	void gameLoop() override {
		if (forward)    moveBy(   cameraSpeed * cameraDirection);
		if (backward)   moveBy( - cameraSpeed * cameraDirection);
		if (right)      moveBy(   cameraSpeed * cameraRight);
		if (left)       moveBy( - cameraSpeed * cameraRight);
		if (up) acceleration += glm::vec3(0,.1f,0);

		physicsLoop();
		glutPostRedisplay();
	}
	virtual void moveBy(glm::vec3 move) override;

	PhysicsCamera(int i, int i1);
};

#endif //OPENGL_PHYSICSCAMERA_H
