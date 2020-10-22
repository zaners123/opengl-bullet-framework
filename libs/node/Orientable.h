#ifndef OPENGL_ORIENTABLE_H
#define OPENGL_ORIENTABLE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


/**An orientable shape has:
 *  - A way to be oriented in OpenGL's format (A 4x4 matrix with orientation and all that)
 *  - A way to br oriented in Bullet's format (a center of mass)
 * This way, a shape can both have weird matrix modifications and still be able to be moved by its center of mass.
 *
 * OpenGL scale/multiplication is applied first, then center of mass.
 * */
class Orientable {
public:
	Orientable() {
//		replaceRigidBody();
	}

	glm::mat4 pos = glm::mat4(1);
	glm::vec3 com = glm::vec3(0,0,0);
	glm::mat4 orientation = glm::mat4(1);

	/**Applies OpenGL mat4 position multiplication only*/
	/*glm::mat4 applyOpenglOnly(const glm::mat4& wvp) {
		return wvp * pos;
	}*/
	glm::vec3 applyPosOnly(const glm::vec3& wvp) {
		glm::vec4 p = glm::vec4(wvp.x,wvp.y,wvp.z,1);
		p = pos * p;
		return glm::vec3(p.x,p.y,p.z);
	}

	glm::mat4 applyPos(const glm::mat4& wvp) {
		return ((glm::translate(wvp,com) * orientation) * pos);
	}

	virtual void replaceRigidBody() {

	}

	virtual void setPos(const glm::mat4& pos) {
		this->pos = pos;
		replaceRigidBody();
	}

	virtual void build() {
		//remeshes, etc
		replaceRigidBody();
	}

	virtual glm::mat4 getPos() {
		return this->pos;
	}

	//-------------------------------------------------------------------
	// some example common modifications
	//-------------------------------------------------------------------

	virtual void scale(GLfloat factor) {
		scale(factor,factor,factor);
	}

	virtual void scale(GLfloat x, GLfloat y, GLfloat z) {
		setPos(glm::scale(getPos(), glm::vec3(x,y,z)));
	}
};
#endif //OPENGL_ORIENTABLE_H
