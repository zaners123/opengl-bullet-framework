//
// Created by bean on 9/23/20.
//

#ifndef OPENGL_CAMERACONTROLLER_H
#define OPENGL_CAMERACONTROLLER_H

#include "../camera/camera.h"

class CameraController {
public:
	Camera* c;
	explicit CameraController(Camera* cam) {
		c = cam;
	}

	void keyup(unsigned char key, int x, int y);
	void keydown(unsigned char key, int x, int y);
};
#endif //OPENGL_CAMERACONTROLLER_H
