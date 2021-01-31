#ifndef OPENGL_DRAWABLE_H
#define OPENGL_DRAWABLE_H

#include <iostream>

/**
 * Something that can be drawn. Also has variables you can use like vao, prog, etc.
 * */
class Drawable {

protected:
	GLuint  vao;
	GLuint  vbo;
	GLuint  ebo;
	GLuint  tex;
	bool progLock = false;
	GLuint prog = 0;
public:
	Drawable() = default;
	~Drawable() = default;

	virtual void drawDrawable(glm::mat4 wvm) {
		std::cerr<<"We're doing this wrong"<<std::endl;
	};

	const GLuint getProg() {return prog;};
};

#endif //OPENGL_DRAWABLE_H
