#ifndef OPENGL_NODE_H
#define OPENGL_NODE_H

//#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "vector"
#include "Drawable.h"
#include "Collidable.h"

/**
 * A Node is a Drawable but with a wvp offset, so it could have functions like move(5,0,0) or scale(2,2,2)
 * */
class Node : protected Drawable, public Collidable {
protected:
	glm::mat4 pos = glm::mat4(1);
	virtual void drawDrawable(glm::mat4 wvp) = 0;
	//todo implement magic offset functions such as move or scale (or just give it an internal mat4)
	glm::mat4 applyPos(glm::mat4 wvp) {
		return wvp * pos;
	}
	glm::vec3 applyPos(glm::vec3 wvp) {
		glm::vec4 p = glm::vec4(wvp.x,wvp.y,wvp.z,0);
		p = pos * p;
		return glm::vec3(p.x,p.y,p.z);
	}
public:
	Node() {}

	const void draw(glm::mat4 wvp) {
		drawDrawable(applyPos(wvp));
	}

	virtual Node* setPos(glm::mat4 pos) {this->pos = pos;return this;}
	virtual glm::mat4 getPos() {return this->pos;}

	//common modifications
	Node* move(GLfloat x, GLfloat y, GLfloat z) {
		setPos(glm::translate(getPos(), glm::vec3(x,y,z)));
		return this;
	}
	Node* scale(GLfloat factor) {
		setPos(glm::scale(getPos(), glm::vec3(factor,factor,factor)));
		return this;
	}
	Node* scale(GLfloat x, GLfloat y, GLfloat z) {
		setPos(glm::scale(getPos(), glm::vec3(x,y,z)));
		return this;
	}
};

#endif //OPENGL_NODE_H
