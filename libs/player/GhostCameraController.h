//
// Created by bean on 9/23/20.
//

#ifndef OPENGL_GHOSTCAMERACONTROLLER_H
#define OPENGL_GHOSTCAMERACONTROLLER_H

#include "../camera/camera.h"
#include "CameraController.h"

class GhostCameraController : public CameraController {
public:
	GhostCameraController(Camera* c) : CameraController(c) {

	}

	/*void static keyup(unsigned char key, int x, int y) {
		if (c->toggleKeys(key,false)) {
			glutPostRedisplay();
			return;
		}
		glutPostRedisplay();
	}

	void static keydown(unsigned char key, int x, int y) {
		if (c->toggleKeys(key,true)) {
			glutPostRedisplay();
			return;
		}
		glutPostRedisplay();
	}*/
};
#endif //OPENGL_GHOSTCAMERACONTROLLER_H
