#ifndef OPENGL_CAMERA_HOLD
#define OPENGL_CAMERA_HOLD

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include "../node/Collidable.h"


/**
 *
 * A camera is an object that represents a player, used to view the world from their perspective
 *
Howto use:

 before glutMainLoop, put"
    cam = new Camera(2000,1000);
    glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
    glutReshapeFunc(cam->reshape);
	glutPassiveMotionFunc(cam->rotateWorld);
	glutSetCursor(GLUT_CURSOR_NONE);
 "

 end with something like "
    glutDisplayFunc(display);
	glutTimerFunc(100, drawLoop, 0);
    glutFullScreen();
	glutMainLoop();
 "


add "
	void keydown(unsigned char key, int x, int y) {
		cam->toggleKeys(key,true);
	   glutPostRedisplay();
	}
	void keyup(unsigned char key, int x, int y) {
		cam->toggleKeys(key,false);
	   glutPostRedisplay();
	}
	void drawLoop(int n) {
	   cam->drawLoop();
	   glutTimerFunc(40, drawLoop, n);
	}
"
 * */
class Camera {
	glm::vec3 location;
	constexpr static float PI = 3.14159265358979323;
	static int windowX,windowY;
	static float cameraSpeed;//walking speed
	static float mouseSensitivity;//spinning speed
	static float rotSensitivity;//q and e screen rotate
	static float rot;
	static bool left,right,up,down,forward,backward,movRot,movCounterRot,boost,slow;
	static float xAngle;//where mouse is pointed at, now in 3d!
	static float yAngle;
	static glm::mat4 model;//WVP
	//where the player is
	static glm::vec3 cameraDirection;
	static glm::vec3 cameraRight;
	static glm::vec3 upVec;
public:
	Camera(int initW, int initH);

	/**
	 * Call this if the viewport is resized.
	 * Ex: 	glutReshapeFunc(cam->reshape);
	 * */
	static void reshape(int x, int y);

	/**@brief Used for keys where you care if they're held down
	 * @param key the letter pressed
	 * @param set if true, it was pressed. If false, it was let go of
	 * */
	static bool toggleKeys(unsigned char key, bool set);

	/**
	 * If you multiply a 3D shape by this, the result will be the 3D shape from their perspective.
	 * */
	glm::mat4 getWVP();

	virtual void moveBy(glm::vec3 move);

	/**
	 * Run this before drawing every frame, so the camera can move
	 * */
	virtual void gameLoop();
	/**
	 * Pans camera. Call this when the mouse moves. It moves the mouse back, and changes the camera orientation.
	 * Ex: 	glutPassiveMotionFunc(cam->rotateWorld);
	 * @param int x
	 * @param int y
	 * */
	static void rotateWorld(int x, int y);
	/**Call this when the user releases a key.
	 * Ex: 	glutKeyboardUpFunc(cam->keyup);
	 * */
	static void keyup(unsigned char key, int x, int y);
	/**Call this when the user presses a key.
	 * Ex: 	glutKeyboardFunc(cam->keydown);
	 * */
	static void keydown(unsigned char key, int x, int y);

	glm::vec3 getLoc();
};

#endif //OPENGL_CAMERA_HOLD
