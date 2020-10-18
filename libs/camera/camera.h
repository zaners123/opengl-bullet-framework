#ifndef OPENGL_CAMERA_HOLD
#define OPENGL_CAMERA_HOLD

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include "../node/Collidable.h"


/**
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
//	glutFullScreen();
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

 */

class Location {
public:
	glm::vec3 location;
};

/**
 * A camera is an object with a location that can be moved and stuff
 * */
class Camera : public virtual Location {
public:
	constexpr static float PI = 3.14159265358979323;
	static int windowX,windowY;
	static float cameraSpeed;//walking speed
	static float mouseSensitivity;//spinning speed
	static float rotSensitivity;//q and e screen rotate
	static bool virt;//was mouse movement virtual? this stops the function from calling itself every like 0ms. Really just ignores every other mouse movement...
	static float rot;
	static bool left,right,up,down,forward,backward,movRot,movCounterRot,boost,slow;
	static float xAngle;//where mouse is pointed at, now in 3d!
	static float yAngle;
	static glm::mat4 model;//WVP
	//where the player is
	static glm::vec3 cameraDirection;
	static glm::vec3 cameraRight;
	static glm::vec3 upVec;
	Camera(int initW, int initH);
	static void reshape(int x, int y);
	/**@brief Used for keys where you care if they're held down
	 * @param key the letter pressed
	 * @param set if true, it was pressed. If false, it was let go of
	 * */
	static bool toggleKeys(unsigned char key, bool set);
	glm::mat4 getWVP();
	virtual void moveBy(glm::vec3 move);

	virtual void gameLoop();
	//pans camera (by mean of model matrix)
	static void rotateWorld(int x, int y);
	virtual bool isCollision(glm::vec3 point);
	static void keyup(unsigned char key, int x, int y);
	static void keydown(unsigned char key, int x, int y);
};

#endif //OPENGL_CAMERA_HOLD
