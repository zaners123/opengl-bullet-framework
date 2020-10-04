#ifndef OPENGL_ORIENTABLE_H
#define OPENGL_ORIENTABLE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

/***/
class Orientable {
public:
	glm::mat4 pos = glm::mat4(1);

	glm::mat4 applyPos(glm::mat4 wvp) {
		return wvp * pos;
	}
	glm::vec3 applyPos(glm::vec3 wvp) {
		glm::vec4 p = glm::vec4(wvp.x,wvp.y,wvp.z,0);
		p = pos * p;
		return glm::vec3(p.x,p.y,p.z);
	}

	virtual void setPos(glm::mat4 pos) {
		this->pos = pos;
	}
	virtual glm::mat4 getPos() {
		return this->pos;
	}

	//some example common modifications
	virtual void move(GLfloat x, GLfloat y, GLfloat z) {
		setPos(glm::translate(getPos(), glm::vec3(x,y,z)));
	}

	virtual void scale(GLfloat factor) {
		setPos(glm::scale(getPos(), glm::vec3(factor,factor,factor)));
	}

	virtual void scale(GLfloat x, GLfloat y, GLfloat z) {
		setPos(glm::scale(getPos(), glm::vec3(x,y,z)));
	}
};
#endif //OPENGL_ORIENTABLE_H
