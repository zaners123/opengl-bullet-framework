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
#include <thread>
//my stuff
#include "../libs/camera/camera.h"
#include "../libs/node/Maze.h"
#include "../libs/node/Cube.h"
#include "../libs/Timer.h"
#include "../libs/node/NodeVector.h"
#include "../libs/node/Sphere.h"
#include "../libs/node/QuarterBowl.h"
#include "../libs/node/NodeBuilder.h"

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

void setupRootNode() {
	rootNode = new NodeVector();
	rootNode->setPhysicsWorld(dynamicsWorld);
	//make bowl
	for (int i=0;i<4;i++) {
		auto bowl = new QuarterBowl();
		bowl->setFixed();
		bowl->setTexture("../wood.jpeg");
		bowl->move(0,-100,0);
		bowl->scale(10);
		bowl->setPos(glm::rotate(bowl->getPos(), (float)(i * M_PI / 2), glm::vec3(0, 1, 0)));
		rootNode->push(bowl);
	}
	//make table
	auto* table = NodeBuilder::start()
			.setShape(NodeBuilder::cube)
			->setTexture("../marble.jpg")
			->build();
	table->setFixed();
	table->move(0,-161.0f,0);
	table->scale(100,100,100);
	rootNode->push(table);
	//skybox
	auto* skybox = NodeBuilder::start()
			.setShape(NodeBuilder::sphere)
			->setTexture("../sky.jpeg")
			->build();
	skybox->scale(3000);
	rootNode->push(skybox,false);
	//make fruit
	for (int i=0;i<8;i++) {
		auto* s = new Sphere();
		int r = i%3;
		if (r==0)s->setTexture("../appleTex2.jpg");
		if (r==1)s->setTexture("../mango.jpg");
		if (r==2)s->setTexture("../green_apple.jpg");
//		if (r==3)s->setTexture("../orange2.png");
		s->move(rand()%10-5,0+i*10,rand()%10-5);
		s->scale(2.5f);
		rootNode->push(s);
	}
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

void onFrame() {
	//frame-logic, such as making a new object every 10 frames

	//todo uncomment this to spawn a fruit every frame

	static int i=0;
	static int count=0;
	if (count<300 && i++>1) {
		i=0;
		auto* s = new Sphere();
		if (count%5==0) s->setTexture("../appleTex2.jpg");
		if (count%5==1) s->setTexture("../orange2.png");
		if (count%5==2) s->setTexture("../green_apple.jpg");
		if (count%5==3) s->setTexture("../mango.jpg");
		if (count%5==4) s->setTexture("../lemon.png");
		s->move(rand()%10-5,580+i*5,rand()%10-5);
		s->scale((((float)(rand()%1000))/1000)*3+1.0f);
		rootNode->push(s);
		count++;
		if (count%100==0) {
			std::cout<<"FRUIT:"<<count<<std::endl;
		}
	}
}

void physicsLoop(int n) {
	glutTimerFunc(10, physicsLoop, n);
	dynamicsWorld->stepSimulation(1.f/60.f,10);
	rootNode->updateUsingRigidBody();
	onFrame();
}

void drawLoop(int n) {
	glutTimerFunc(20, drawLoop, n);
	cam->gameLoop();
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
	glutDisplayFunc(display);
	glutSetCursor(GLUT_CURSOR_NONE);
	//other stuff
	glutTimerFunc(100, drawLoop, 0);
	glutTimerFunc(100, physicsLoop, 0);
	glutFullScreen();
//	std::thread physicsThread(physicsLoopThread);
	glutMainLoop();
	return 0;
}