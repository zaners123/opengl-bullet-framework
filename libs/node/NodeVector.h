
#ifndef OPENGL_NODEVECTOR_H
#define OPENGL_NODEVECTOR_H
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Node.h"
#include <vector>
/**
 * NodeVector is a wrapper for Node.
 *      If you call "Move", it moves all its children;
 *      If you call "Draw", it draws all its children
 * */
class NodeVector : public Node {
 	std::vector<Node*> children;
 	btDiscreteDynamicsWorld* world;
public:
 	NodeVector() = default;

	virtual void setPhysicsWorld(btDiscreteDynamicsWorld* newWorld) override {
		world = newWorld;
		for(Node* n : children) {
			n->setPhysicsWorld(world);
		}
	}

	/**Remove node from NodeVector. Returns true on success
	 * @param Node node - node in NodeVector to remove
	 * @return true if found & removed
	 * */
	bool remove(Node* node) {
		if (node==nullptr) return false;
		auto toErase=std::find(children.begin(), children.end(), node);
		if (!(toErase != children.end())) return false;
		//if (*toErase != nullptr) delete *toErase ;
		children.erase(toErase);
		return true;
	}

	/**Pushes Node to NodeVector
	 * @returns true on success. False on error
	 *      Error could be from:
	 *          If addToPhysicsWorld=true but you didn't set the physics world
	 *          If you didn't give the Node node a RigidBody
	 * */
 	bool push(Node* node, bool addToPhysicsWorld = true) {
		if (addToPhysicsWorld) {
			if (!world) {
				std::cerr << "Physics world is not set for this NodeVector" << std::endl;
				return false;
			}
			btRigidBody* rigidBody = node->getRigidBody();
			if (!rigidBody) {
				std::cerr << "Node hath no Body" << std::endl;
				return false;
			}
			world->addRigidBody(rigidBody);
		}
	    children.push_back(node);
	    return true;
 	}
	void updateUsingRigidBody() override {
		for(Node* n:children) {
			n->updateUsingRigidBody();
		}
 	}
	void drawDrawable(glm::mat4 wvp) override {
 		for (Node* n : children) {
 			n->draw(wvp);
 		}
 	}
	 void clear() {
 		children.clear();
 	}
 };
#endif //OPENGL_NODEVECTOR_H