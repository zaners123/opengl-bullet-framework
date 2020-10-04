//
// Created by zaners123 on 7/16/20.
//

#ifndef OPENGL_NODEVECTOR_H
#define OPENGL_NODEVECTOR_H

//#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Node.h"
#include "vector"

/**
 * NodeVector is a wrapper for Node.
 *      If you call "Move", it moves all its children;
 *      If you call "Draw", it draws all its children
 * */
class NodeVector : public Node {
 	std::vector<Node*> children;
public:
 	NodeVector() = default;;
 	void push(Node* node) {
 		children.push_back(node);
 	}
 	void drawDrawable(glm::mat4 wvp) override {
 		for (Node* n : children) {
 			n->draw(wvp);
 		}
 	}
 	/*BoundingBox getBoundingBox() {
 		BoundingBox box;
 		if (children.empty()) return box;
 		box = children.at(0)->getBoundingBox();
 		for (Node* n : children) {
 			box = BoundingBox::encapsulateInsideBox(box,n->getBoundingBox());
 		}
 		return box;
 	}*/
 	/*bool isCollision(glm::vec3 point) {
	    point = applyPos(point);
	    for (Node* n : children) {
		    if (n->isCollision(point)) return true;
	    }
	    return false;
 	}*/

	 void clear() {
 		children.clear();
 	}
 };
#endif //OPENGL_NODEVECTOR_H
