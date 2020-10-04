#ifndef OPENGL_NODE_H
#define OPENGL_NODE_H

//#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "vector"
#include "Drawable.h"
#include "Collidable.h"
#include "Orientable.h"

/**
 * A Node is:
 *  - Drawable (can call draw() on it given a wvp matrix)
 *  - Orientable (can move/scale it around, so it could have functions like move(5,0,0) or scale(2,2,2).)
 *  - Collidable (Has a btRigidBody that it can base its position off of)
 * */
class Node : protected Drawable, public Collidable, public Orientable {
protected:
	virtual void drawDrawable(glm::mat4 wvp) = 0;
	//todo implement magic offset functions such as move or scale (or just give it an internal mat4)
public:
	Node() {}

	void updateUsingRigidBody() override {}

	const void draw(glm::mat4 wvp) {
		drawDrawable(applyPos(wvp));
	}

	//common modifications - stackable
	Node* getMove(GLfloat x, GLfloat y, GLfloat z) {
		Orientable::move(x,y,z);
		return this;
	}
	Node* getScale(GLfloat factor) {
		Orientable::scale(factor);
		return this;
	}
	Node* getScale(GLfloat x, GLfloat y, GLfloat z) {
		Orientable::scale(x,y,z);
		return this;
	}
};

#endif //OPENGL_NODE_H
