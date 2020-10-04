#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <vector>
#include "../camera/camera.h"
#include "WorldHex/World.h"
#include "PhysicsCamera.h"

//objects
//PhysicsCamera* cam;
Camera* cam;
World* world;

//functions
void init() {
	cam = new Camera(2000,1000);
	Camera::cameraSpeed = .1f;

	long seed = 15;//randomly generated using my brain


	world = new World(seed);

	glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
//	glCullFace(GL_FRONT);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto wvp = cam->getWVP();

	world->draw(wvp);

	glutSwapBuffers();
//	glFlush();
}

void keyup(unsigned char key, int x, int y) {
	if (cam->toggleKeys(key,false)) {
		glutPostRedisplay();
		return;
	}
	glutPostRedisplay();
}

void keydown(unsigned char key, int x, int y) {
	if (cam->toggleKeys(key,true)) {
		glutPostRedisplay();
		return;
	}
	glutPostRedisplay();
}

void gameLoop(int n) {
	cam->gameLoop();
	world->updatePlayerLoc(cam->location);

	glutTimerFunc(20, gameLoop, n);
	glutPostRedisplay();
}

void onClick(int button, int state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		std::cout<<"CLICK"<<state<<std::endl;
		blockWorldLoc loc = world->getBlockPointedAt(cam->location,Camera::cameraDirection);

		std::cout<<"BLOCK LOC "<<(int)std::get<0>(loc.second)<<(int)std::get<1>(loc.second)<<(int)std::get<2>(loc.second)<<std::endl;


		*(world->getBlockRef(loc)) = stone;
//		for (int x=0;x<10;x++) {world->getChunk(loc.first)->block[x][0][0] = dirt;}
		world->getChunk(loc.first)->genMesh();
		world->getChunk(loc.first)->pm->update();
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hex");

	if (glewInit()) {
		printf("%s\n", "Unable to initialize GLEW ...");
		return 1;
	}

	init();
	/* we can now get data for the specific OpenGL instance we created */
	printf("GL Vendor : %s\n", glGetString(GL_RENDERER));
	printf("GL Renderer : %s\n", glGetString(GL_VENDOR));
	printf("GL Version (string) : %s\n", glGetString(GL_VERSION));
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//camera library
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutReshapeFunc(cam->reshape);
	glutPassiveMotionFunc(cam->rotateWorld);
	glutSetCursor(GLUT_CURSOR_NONE);
	//other stuff
	glutMouseFunc(onClick);
	glutDisplayFunc(display);
	glutTimerFunc(100, gameLoop, 0);
	glutFullScreen();//todo fullscreen
	glutMainLoop();
	return 0;
}