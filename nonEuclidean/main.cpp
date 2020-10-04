//magic OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
//normal C stuff
#include <cstdio>
#include <stack>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
//my stuff
#include "../libs/camera/camera.h"
#include "../libs/node/Maze.h"
#include "../libs/node/Plane.h"
#include "../libs/node/PointMaze.h"
#include "../libs/node/Cube.h"
#include "../libs/Timer.h"
#include "../libs/node/NodeVector.h"
#include "../libs/node/Portal.h"
#include "../libs/node/Cylinder.h"
#include "../libs/node/Sphere.h"
#include "../libs/player/GhostCameraController.h"


Camera* cam;
NodeVector* rootNode;
btDefaultCollisionConfiguration* collisionConfig;
btDispatcher* dispatcher;
btDbvtBroadphase* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

/*void testBullet() {
	//make an environment
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));


	//make a falling cube
	btBoxShape* boxCollisionShape = new btBoxShape(btVector3(btScalar(5.), btScalar(5.), btScalar(5.)));
	btScalar boxMass = 1;
	btDefaultMotionState* boxMotionState = new btDefaultMotionState();
	btVector3 localInertia(0, 0, 0);
	btRigidBody* fallingCubeRB = new btRigidBody(boxMass, boxMotionState, boxCollisionShape, localInertia);
	btTransform rbLoc = fallingCubeRB->getCenterOfMassTransform();
	rbLoc.setOrigin(rbLoc.getOrigin()+btVector3(0,500,0));
	fallingCubeRB->setCenterOfMassTransform(rbLoc);
	boxCollisionShape->calculateLocalInertia(boxMass, localInertia);
	dynamicsWorld->addRigidBody(fallingCubeRB);


	//make a stationary platform
	btStaticPlaneShape* floorShape = new btStaticPlaneShape(btVector3(0, 1, 0)*//*Normal*//*, 0*//*Thickness*//*);
	btScalar floorMass = 0;//won't move
	btDefaultMotionState* floorMotionState = new btDefaultMotionState();
	btVector3 floorInertia(0,0,0);
	btRigidBody* floorRB = new btRigidBody(floorMass, floorMotionState, floorShape, floorInertia);
	dynamicsWorld->addRigidBody(floorRB);

	//simulate!
	btVector3 pos;
	for (int x=0;x<1000;x++) {
		pos=fallingCubeRB->getCenterOfMassPosition();
		std::cout<<pos.x()<<','<<pos.y()<<','<<pos.z()<<'\n';
		dynamicsWorld->stepSimulation(1.f/60.f,10);
	}
}*/

void initBullet() {
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

void addToPhysicsWorld(Node* node) {
	btRigidBody* rigidBody = node->getRigidBody();
	if (rigidBody) {
		dynamicsWorld->addRigidBody(rigidBody);
	} else {
		std::cerr<<"Node hath no Body"<<std::endl;
	}
}

void stepBullet() {
	dynamicsWorld->stepSimulation(1.f/60.f,10);
}

void setupRootNode() {
	initBullet();

	rootNode = new NodeVector();
	Cube* c = new Cube(true);
	c->scale(5.0f);
	addToPhysicsWorld(c);
	rootNode->push(c);

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
	outside->push((new Cube(true))->move(55, 0, 0)->scale(10,10,120));
	outside->push((new Cube(true))->move(-55, 0, 0)->scale(10,10,120));
	outside->push((new Cube(true))->move(0, 0, 55)->scale(100,10,10));
	outside->push((new Cube(true))->move(0, 0, -55)->scale(100,10,10));
	//put stuff outside
	outside->push((new Cube(true))->move(50, 20, 0)->scale(10));
	outside->push((new Cylinder(1000))->move(0,30,0)->scale(10));
	//put stuff inside
	inside->push((new Cube(true))->scale(10)->move(0,-10,0));
	inside->push((new Sphere())->move(0,-5,0));*/
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
	drawRootNode();
	glFlush();
//	glutPostRedisplay();
}

void physicsLoop() {
	stepBullet();
}

void gameLoop(int n) {
	glutTimerFunc(20, gameLoop, n);
	cam->gameLoop();
	physicsLoop();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_STENCIL|GLUT_DEPTH);
	glutCreateWindow("The Portal");
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