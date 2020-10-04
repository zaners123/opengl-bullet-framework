#ifndef OPENGL_PORTAL_H
#define OPENGL_PORTAL_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Node.h"

class Portal : public Node {

	Node* portalFrameNode;
	Node* insideNode;
	Node* outsideNode;
public:
	Portal(Node* portalFrameNode, Node* insideNode, Node* outsideNode) {
		this->portalFrameNode = portalFrameNode;
		this->insideNode = insideNode;
		this->outsideNode = outsideNode;
	}

	//todo?
	bool isCollision(glm::vec3 point) {return false;}

	void drawDrawable(glm::mat4 wvp) {
		drawPortal(wvp);
	}

	/**Draws a (half) portal. The reason I say half portal is because think of a full portal like nothing (go through it and you're in the same location),
	 *      but think of a half portal like you go through it (from outside to inside) and you end up somewhere else
	 *
	 * */
	void drawPortal(glm::mat4 wvp) {
		glEnable(GL_STENCIL_TEST);
		//write given info the stencil buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_LESS);
		glStencilFunc(GL_NEVER, 1, 0xFF);//everything fails stencil test
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);//draw 1's on test fail (always)
		//"Clear" stencil buffer (write ones everywhere)
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
		//draw allowed bounds
		portalFrameNode->draw(wvp);
		glStencilMask(0x00); // disable writing to the stencil buffer
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthFunc(GL_LESS);
		glStencilFunc(GL_EQUAL, 1, 0xFF);//draw only when it's one
		insideNode->draw(wvp);
		//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//draw only when it's one
		//	drawOutside();
		glDisable(GL_STENCIL_TEST);
		//write portal, for depth buffer
		//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		portalFrameNode->draw(wvp);
		//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		outsideNode->draw(wvp);
	}
};
#endif //OPENGL_PORTAL_H
