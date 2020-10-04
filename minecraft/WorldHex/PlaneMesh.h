//
// Created by zaners123 on 5/2/20.
//

#ifndef OPENGL_PLANEMESH_H
#define OPENGL_PLANEMESH_H

#include <vector>
#include "../../camera/camera.h"

//typedef int chunkRefType;
//#define CHUNK_GL GL_INT
typedef unsigned char blockIndexType;
#define CHUNK_GL GL_BYTE

static std::vector<float> plane = {
		/*
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(1*M_PI/3),0,(float)sin(1*M_PI/3),//1
		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(5*M_PI/3),0,(float)sin(5*M_PI/3),//5
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),//6
		(float)cos(1*M_PI/3),1,(float)sin(1*M_PI/3),//7
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),//8
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),//9
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),//10
		(float)cos(5*M_PI/3),1,(float)sin(5*M_PI/3),//11
		 */

		//bottom face
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(1*M_PI/3),0,(float)sin(1*M_PI/3),//1
		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(5*M_PI/3),0,(float)sin(5*M_PI/3),//5
		//top face
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),
		(float)cos(1*M_PI/3),1,(float)sin(1*M_PI/3),
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),
		(float)cos(5*M_PI/3),1,(float)sin(5*M_PI/3),
		//sides
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(1*M_PI/3),0,(float)sin(1*M_PI/3),//1
		(float)cos(1*M_PI/3),1,(float)sin(1*M_PI/3),//7
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(1*M_PI/3),1,(float)sin(1*M_PI/3),//7
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),//6

		(float)cos(1*M_PI/3),0,(float)sin(1*M_PI/3),//1
		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),//8
		(float)cos(1*M_PI/3),0,(float)sin(1*M_PI/3),//1
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),//8
		(float)cos(1*M_PI/3),1,(float)sin(1*M_PI/3),//7

		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),//9
		(float)cos(2*M_PI/3),0,(float)sin(2*M_PI/3),//2
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),//9
		(float)cos(2*M_PI/3),1,(float)sin(2*M_PI/3),//8

		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),//10
		(float)cos(3*M_PI/3),0,(float)sin(3*M_PI/3),//3
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),//10
		(float)cos(3*M_PI/3),1,(float)sin(3*M_PI/3),//9

		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(5*M_PI/3),0,(float)sin(5*M_PI/3),//5
		(float)cos(5*M_PI/3),1,(float)sin(5*M_PI/3),//11
		(float)cos(4*M_PI/3),0,(float)sin(4*M_PI/3),//4
		(float)cos(5*M_PI/3),1,(float)sin(5*M_PI/3),//11
		(float)cos(4*M_PI/3),1,(float)sin(4*M_PI/3),//10

		(float)cos(5*M_PI/3),0,(float)sin(5*M_PI/3),//5
		(float)cos(0*M_PI/3),0,(float)sin(0*M_PI/3),//0
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),//6
		(float)cos(5*M_PI/3),0,(float)sin(5*M_PI/3),//5
		(float)cos(0*M_PI/3),1,(float)sin(0*M_PI/3),//6
		(float)cos(5*M_PI/3),1,(float)sin(5*M_PI/3),//11
};
static unsigned long planestorage = plane.size()*sizeof(plane.at(0));

class PlaneMesh : public Drawable {
public:
	//todo ebo would save me 33% of vertices, but might be slower in the end

	//todo generate by chunk parsing. Each plane is stored as four location vertices, printed in ccw order
	//todo could store plane as xyz(face)
	//main increment by one for each plane attribute
	const int planeSize = 6;
	const int planeSizeFromDim = 4;
	long planeMaxSize = 1024;//todo what should initial buffer be?
	//todo pass it a cube with a bitmask of faces (geometry shader?)
	std::vector<blockIndexType> planeLoc;
	bool bufferInitialized = false;

	PlaneMesh();

	//todo make vec3 an integer vec3
	/**Called when the mesh should be updated
	 * */
	void update();

	bool planeBufferTooLarge();

	void draw() override;
	void draw(glm::mat4 wvp) override;
	bool isCollision(glm::vec3 low, glm::vec3 high) override;

};

#endif //OPENGL_PLANEMESH_H
