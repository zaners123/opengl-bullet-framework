#include "camera.h"
#include "../node/Drawable.h"

int Camera::windowX,Camera::windowY;
float Camera::cameraSpeed;//walking speed
float Camera::mouseSensitivity = 0.001f;//spinning speed
float Camera::rotSensitivity = 0.1f;//q and e screen rotate
float Camera::rot = 0;
bool Camera::left,Camera::right,Camera::up,Camera::down,Camera::forward,Camera::backward,Camera::movRot,Camera::movCounterRot,Camera::boost,Camera::slow;
float Camera::xAngle;//where mouse is pointed at, now in 3d!
float Camera::yAngle;
glm::mat4 Camera::model;//WVP
glm::vec3 Camera::cameraDirection;
glm::vec3 Camera::cameraRight;
glm::vec3 Camera::upVec;

Camera::Camera(int initW, int initH) {
	location = glm::vec3(0,10,150);
	glutInitWindowSize(initW,initH);
	this->windowX = initW;
	this->windowY = initH;
	xAngle = 0;
	yAngle = 0;
	cameraSpeed = 1.0f;
	cameraDirection = glm::vec3(0, 0, -1.0f);
	upVec = glm::vec3(0, 1.0f, 0.0f);
}

void Camera::gameLoop() {
	if (forward)    moveBy(   cameraSpeed * cameraDirection);
	if (backward)   moveBy( - cameraSpeed * cameraDirection);
	if (right)      moveBy(   cameraSpeed * cameraRight);
	if (left)       moveBy( - cameraSpeed * cameraRight);
	if (up)         moveBy(   cameraSpeed * upVec);
	if (down)       moveBy( - cameraSpeed * upVec);
	if (movRot) rot +=rotSensitivity;
	if (movCounterRot) rot -=rotSensitivity;
	glutPostRedisplay();
}
void Camera::keyup(unsigned char key, int x, int y) {
	if (toggleKeys(key,false)) {
		glutPostRedisplay();
		return;
	}
	glutPostRedisplay();
}
void Camera::keydown(unsigned char key, int x, int y) {
	if (toggleKeys(key,true)) {
		glutPostRedisplay();
		return;
	}
	glutPostRedisplay();
}

void Camera::rotateWorld(int x, int y) {
	//was mouse movement virtual? this stops the function from calling itself every like 0ms. Really just ignores every other mouse movement...
	static bool virt = false;
	//keep mouse in middle
	if (!virt) glutWarpPointer(windowX/2,windowY/2);
	virt = !virt;
	//if it needs to move at all
	if (x == windowX/2 && y == windowY/2) return;
	//center at middle of screen
	x -= windowX/2;
	y -= windowY/2;
	//remember mouse angle
	xAngle -= mouseSensitivity * float(x);
	yAngle -= mouseSensitivity * float(y);
	//stop them from looking up too far
	yAngle = glm::clamp(yAngle,-PI/2.01f,PI/2.01f);

	//rotate with mouse x
	model = rotate(glm::mat4(1.0f), xAngle, glm::vec3(0.0, 1.0, 0.0));
	//rotate with mouse y
	model = rotate(model, yAngle, glm::vec3(1.0, 0.0, 0.0));
	//refresh
	glutPostRedisplay();
}

glm::mat4 Camera::getWVP() {
	cameraDirection = glm::mat3(model) * glm::vec3(0,0,-1.0f);

	glm::mat3 rotMatrix = glm::mat3(rotate(glm::mat4(1.0f), rot, glm::vec3(0,0,1)));
	upVec = rotMatrix * glm::vec3(0,1,0);

	cameraRight = glm::normalize(glm::cross(cameraDirection, upVec));
	glm::mat4 view = glm::lookAt(
			location,//camera loc
			location + cameraDirection,//looking at
			upVec//up direction (for maths)
	);
	//	std::cout<<"\tCAMLOC: ("<<cameraLoc.x<<','<<cameraLoc.y<<','<<cameraLoc.z<<')'<<std::endl;

	//the thing that makes far things further
	glm::mat4 projection = glm::perspective (glm::radians(45.0f), float(windowX)/float(windowY), 0.01f, 5000.0f);

	/* Set MVP matrix */
	return projection * view;
}

/**Used for keys where you care if they're held down
	 * @param key the letter pressed
	 * @set if true, it was pressed. If false, it was let go of
	 * */
bool Camera::toggleKeys(unsigned char key, bool set) {

	switch(key) {
		case 'w':forward        = set;break;
		case 's':backward       = set;break;
		case 'd':right          = set;break;
		case 'a':left           = set;break;
		case ' ':up             = set;break;
		case 'c':down           = set;break;
//		case 'e':movRot         = set;break;
//		case 'q':movCounterRot  = set;break;
		case 'b':boost  = set;break;
		case 'n':slow  = set;break;
		case 'q':
			glutLeaveMainLoop();
//			exit(0);
			break;
		default:return false;
	}
	glutPostRedisplay();
	return true;
}

void Camera::reshape(int x, int y) {
	glViewport(0,0,x,y);
	windowX = x;
	windowY = y;
}

void Camera::moveBy(glm::vec3 move) {
	if(boost) move = move * 25.0f;
	if(slow) move = move  / 25.0f;
	location += move;
}

glm::vec3 Camera::getLoc() {
	return location;
}
