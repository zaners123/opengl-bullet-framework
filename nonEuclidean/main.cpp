//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
//normal C stuff
#include <cstdio>
#include <stack>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btDefaultMotionState.h>
//my stuff
#include "../libs/camera/camera.h"
#include "../libs/node/Maze.h"
#include "../libs/node/Cube.h"
#include "../libs/Timer.h"
#include "../libs/node/NodeVector.h"
#include "../libs/node/Sphere.h"

Camera* cam;
NodeVector* rootNode;
btDefaultCollisionConfiguration* collisionConfig;
btDispatcher* dispatcher;
btDbvtBroadphase* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

void initBullet() {
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

void stepBullet() {
	dynamicsWorld->stepSimulation(1.f/60.f,10);
	rootNode->updateUsingRigidBody();
}

void setupRootNode() {
	rootNode = new NodeVector();
	rootNode->setPhysicsWorld(dynamicsWorld);
	auto* c = new Sphere();
	c->setFixed();
	c->move(0,-50,0);
//	c->setPos(glm::rotate(c->getPos(),(float)(M_PI/1.9f),glm::vec3(1,0,0)));
	c->scale(50);
	rootNode->push(c);

	for (int i=0;i<1;i++) {
		auto* s = new Sphere(5);
		s->move(0,5+i*5,0);
//		s->scale(50);
		rootNode->push(s);
	}

	/*rootNode = new NodeVector();
	rootNode->push(new Cube(true));
	Node* portalFace = new Plane(true);
		portalFace->setPos(glm::rotate(portalFace->getPos(), (float) (M_PI / 2.0f), glm::vec3(1, 0, 0)));
		portalFace->scale(100);
	//make portal
	auto* outside = new NodeVector();
	auto* inside = new NodeVector();
	rootNode->push(new Portal(portalFace, inside, outside));
	//make frame
	outside->push((new Cube(true))->moveR(55, 0, 0)->scaleR(10,10,120));
	outside->push((new Cube(true))->moveR(-55, 0, 0)->scaleR(10,10,120));
	outside->push((new Cube(true))->moveR(0, 0, 55)->scaleR(100,10,10));
	outside->push((new Cube(true))->moveR(0, 0, -55)->scaleR(100,10,10));
	//put stuff outside
	outside->push((new Cube(true))->moveR(50, 20, 0)->scaleR(10));
	outside->push((new Cylinder(1000))->moveR(0,30,0)->scaleR(10));
	//put stuff inside
	inside->push((new Cube(true))->scaleR(10)->moveR(0,-10,0));
	inside->push((new Sphere())->moveR(0,-5,0));*/
}

void init() {
	//background color
	glClearColor(1,1,1,1);
	glLineWidth(10);
	glPointSize(5);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initBullet();
	setupRootNode();
}

//todo set up node system
void drawRootNode() {
	rootNode->draw(cam->getWVP());
}

FPS fps = FPS();
void display() {
	fps.tick();
//	glScissor(0,0,cam->windowX,cam->windowY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//	std::cout<<"DRAW ROOT"<<std::endl;
	drawRootNode();
	glFlush();
//	glutPostRedisplay();
}

void onFrame() {
	/*static int i = 0;
	if (i++ > 10) {
		i=0;*/
		/*auto* s = new Sphere();
		s->move(0,500,0);
		s->setInstantaneousChangeInVelocity(0,-100,0);
		rootNode->push(s);*/
	/*}*/
}

void gameLoop(int n) {
	glutTimerFunc(20, gameLoop, n);
	cam->gameLoop();
	stepBullet();
	onFrame();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_STENCIL|GLUT_DEPTH);
	glutCreateWindow("Apple Bowl");
	if (glewInit()) {
		printf("%s\n", "Unable to initialize GLEW ...");
		return 1;
	}
	ilInit();
	init();
	/* we can now get data for the specific OpenGL instance we created */
	printf("GL Vendor : %s\n", glGetString(GL_RENDERER));
	printf("GL Renderer : %s\n", glGetString(GL_VENDOR));
	printf("GL Version (string) : %s\n", glGetString(GL_VERSION));
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//camera library
	cam = new Camera(2000,1000);
	glutKeyboardFunc(cam->keydown);
	glutKeyboardUpFunc(cam->keyup);
	glutReshapeFunc(cam->reshape);
	glutPassiveMotionFunc(cam->rotateWorld);
	glutSetCursor(GLUT_CURSOR_NONE);
	//other stuff
	glutDisplayFunc(display);
	glutTimerFunc(100, gameLoop, 0);
	glutFullScreen();//todo fullscreen
	glutMainLoop();
	return 0;
}