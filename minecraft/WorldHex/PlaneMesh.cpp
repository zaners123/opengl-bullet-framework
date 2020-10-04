//
// Created by zaners123 on 5/2/20.
//

#include <iostream>
#include "PlaneMesh.h"


void PlaneMesh::draw() {
	glBindVertexArray(vao);
	//todo replace with triangle_fan
	glDrawArraysInstanced(GL_TRIANGLES, 0, plane.size(), planeLoc.size()/planeSize);
//	std::cout<<"DRAWING"<<planeLoc.size()/planeSize<<std::endl;
}

bool PlaneMesh::isCollision(glm::vec3 low, glm::vec3 high) {
	return false;
}

PlaneMesh::PlaneMesh() {

	prog = initShaders(
			"../minecraft/WorldHex/plane.vs",
			"../minecraft/WorldHex/plane.fs");
	glUseProgram(prog);

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);

	update();

	//plane structure
	GLulong offset = 0;
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
	glEnableVertexAttribArray(0);
	offset+=planestorage;
	//main location attribute
	glVertexAttribPointer(2, planeSizeFromDim, CHUNK_GL, GL_FALSE,planeSize*sizeof(blockIndexType), (GLvoid *)offset);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2,1);// new position every time
	//main cubeFace attribute
	glVertexAttribPointer(3, 2, CHUNK_GL, GL_FALSE,planeSize*sizeof(blockIndexType), (GLvoid *)(offset + planeSizeFromDim * sizeof(blockIndexType)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3,1);// new cubeFace every time

	//add texture to the boyo
	/*glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
//	SOIL_load_image


}


void PlaneMesh::update() {

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLulong offset = 0;

	//increase planeMaxSize when overflowing
	//todo variable buffer
	if (!bufferInitialized || planeBufferTooLarge()) {
		//	glUnmapBuffer(GL_ARRAY_BUFFER);

		while (planeBufferTooLarge()) {
			planeMaxSize = (planeMaxSize * 6) / 5;//increase by 20%
		}
		glBufferData(GL_ARRAY_BUFFER, planestorage+planeMaxSize, 0, GL_STATIC_DRAW);
		//add structure of simple plane
		glBufferSubData(GL_ARRAY_BUFFER, 0, planestorage, &plane[0]);
	}

	offset+=planestorage;
	//make the actual update
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(blockIndexType) * planeLoc.size(), &planeLoc[0]);
}

bool PlaneMesh::planeBufferTooLarge() {
	return sizeof(blockIndexType) * planeLoc.size() > planeMaxSize;
}

void PlaneMesh::draw(glm::mat4 wvp) {

}
