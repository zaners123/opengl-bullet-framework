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
#include "../libs/node/NodeBuilder.h"
#include "../libs/node/meshes/Grass.h"

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

glm::vec3 getPlayerLoc() {
	return cam->getLoc();
}

constexpr int NUM_APPLES = 250;
constexpr int TREES_DIM = 5;
SimpleNode* apples[NUM_APPLES];

[[nodiscard]] glm::vec3 treeSpot(int x, int z) {
	return glm::vec3((x-TREES_DIM/2) * 120, 0, z * 40);
}

SimpleNode* bowl[4];

glm::vec3 appleSpot() {
	glm::vec3 ret = treeSpot(rand()%TREES_DIM,rand()%TREES_DIM);

	glm::vec3 locs[] = {
			glm::vec3(-10,20,0),
			glm::vec3(10,23,0),
	};

	ret += locs[rand()%locs->length()];
	ret+=glm::vec3(rand()%3,rand()%3,rand()%3);

	return ret;
}

Sphere* spawnApple(glm::vec3 loc, int mass) {
	Sphere* ret  = new Sphere(25);
	ret ->setMass(mass);
	ret ->setTexture("resource/image/appleTex2.jpg");
	ret ->setCOM(loc);
	ret ->scale(.5f);
	rootNode->push(ret);
	return ret;
}

void orchard() {
	//main apples
	for (int i=0;i<NUM_APPLES;i++) {
		apples[i] = spawnApple(appleSpot(),i<NUM_APPLES/2?1:0);
	}
	//main grass
	auto grass = new Cone(5,false);

	grass->scale(0.2f,2.0f,0.2f);
	grass->setPos(glm::rotate(grass->getPos(), (float)(3 * M_PI / 2), glm::vec3(1, 0, 0)));
	long groundLen = 800;
	for (auto x=0; x < 1000000l; x++) {
		long diam = 20000;
		grass->addInstance(glm::vec3((rand()%(2*diam)-diam)/10.0, (rand()%(2*diam)-(diam))/10.0, 0));
	}
	grass->replaceRigidBody();
	grass->fillBuffers();
	grass->setExternalProg(loadShader("libs/node/shader/external/grass.vs", "libs/node/shader/external/grass.fs"));
	rootNode->push(grass,false);

	//main tree
	Tree* tree = (Tree*)NodeBuilder::start()
			.setShape(NodeBuilder::tree)
			->setTexture("resource/image/would.png")
			->setFixed()
			->build();

	auto* leaf = new Sphere(6,4);
	leaf->setTexture("resource/image/leaf.jpg");
	//leaf->scale(2);
	leaf->setFixed();

	for (int x=0; x < TREES_DIM; x++) {
		for (int z=0; z < TREES_DIM; z++) {
			glm::vec3 spot = treeSpot(x,z);
			tree->addInstance(spot);
			//add leaves
			for (SimpleNode::Point p : tree->leaves) {
				leaf->addInstance(glm::vec3((spot.x+.5f*p.x),(spot.y+.5f*p.y),(spot.z+.5f*p.z)));
			}
		}
	}

	leaf->fillBuffers();
	rootNode->push(leaf,false);

	tree->fillBuffers();
	rootNode->push(tree,false);

	//main ground bowl
	spawnApple(glm::vec3(6,20,50),true);
	spawnApple(glm::vec3(5,30,50),true);
	for (int i=0;i<4;i++) {
		auto* b = NodeBuilder::start()
				.setShape(NodeBuilder::quarterBowl)
				->setFixed()
				->setTexture("resource/image/wood.jpeg")
				->build();
		b->scale(2);
		b->setCOM(5,2,50);
		b->setPos(glm::rotate(b->getPos(), (float)(i * M_PI / 2), glm::vec3(0, 1, 0)));
		rootNode->push(b);
	}
	//main holding bowl
	/*for (int i=0;i<4;i++) {
		bowl[i] = NodeBuilder::start()
				.setShape(NodeBuilder::quarterBowl)
				->setFixed()
				->setTexture("resource/image/wood.jpeg")
				->build();
		bowl[i]->scale(2);
		bowl[i]->setPos(glm::rotate(bowl[i]->getPos(), (float)(i * M_PI / 2), glm::vec3(0, 1, 0)));
		rootNode->push(bowl[i]);
	}*/
	//main ground
	auto* ground = NodeBuilder::start()
			.setShape(NodeBuilder::cube)
			->setTexture("resource/image/grass.jpg")
			->setFixed()
			->build();
	ground->move(0, -50.0f, 0);
	ground->scale(groundLen, 100, groundLen);
	rootNode->push(ground);
	//main skybox
	auto* skybox = NodeBuilder::start()
			.setShape(NodeBuilder::sphere)
			->setTexture("resource/image/sky.jpeg")
			->build();
	skybox->rotate(M_PI,0,0);
	skybox->scale(3000);
	rootNode->push(skybox,false);
}

void orchardFrame() {

	//do nothing most frames
	static int skip=0;while (skip++ < 500/NUM_APPLES) return;skip=0;
	//let all apples move if they wanna
	static int appleI=0;

	for (auto* a : apples) {
		if (!a) continue;
//		if ((i+appleI+NUM_APPLES)%NUM_APPLES < 250) {
			a->rb->setActivationState(DISABLE_DEACTIVATION);
//		} else {
//			a->rb->setActivationState(DISABLE_SIMULATION);
//		}
	}

	int update = appleI;//rand()%NUM_APPLES;
	if (apples[update]->mass>0) {
		apples[update]->setCOM(appleSpot());
	}
//	apples[NUM_APPLES-update-1]->setCOM(appleSpot());
//	apples[NUM_APPLES-update-1]->setMass(1);
//	apples[NUM_APPLES-update-1]->setLinearVelocity(0,5,0);
	appleI= (appleI + 1) % NUM_APPLES;
}

void setupRootNode() {
	rootNode = new NodeVector();
	rootNode->setPhysicsWorld(dynamicsWorld);
	orchard();
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


//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	drawRootNode();

	glutSwapBuffers();
//	glFlush();

//	glutPostRedisplay();
}

void onFrame() {
	//frame-logic, such as making a new object every 10 frames
	orchardFrame();
}

void physicsLoop(int n) {
	glutTimerFunc(10, physicsLoop, n);
	dynamicsWorld->stepSimulation(1.f/60.f,10);
	rootNode->updateUsingRigidBody();
	onFrame();
}

void onDraw() {
	for (int a=0;a<4;a++) if (bowl[a]) bowl[a]->setCOM(getPlayerLoc() + glm::vec3(glm::vec4(0,-.5f,5,0) * cam->getWVP()));
}

void drawLoop(int n) {
	glutTimerFunc(20, drawLoop, n);
	cam->gameLoop();
	onDraw();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_STENCIL|GLUT_DEPTH|GLUT_DOUBLE);
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