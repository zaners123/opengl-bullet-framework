#ifndef OPENGL_WRAPPER_H
#define OPENGL_WRAPPER_H

#include <vector>
#include <glm/vec3.hpp>
#include <iostream>
#include "objloader.h"

struct DrawableData {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;
	glm::vec3* vNormals;
};

objloader loader;
DrawableData* readOBJ(const char* path) {
	auto ret = new DrawableData();

	if (!loader.load(path)) {
		std::cerr<<"PATH NOT FOUND "<<path<<std::endl;
		exit(1);
	}
	ret->vertices = loader.getVertices();
	ret->normals = loader.getNormals();
	ret->indices = loader.getVertexIndices();
	int numverts = ret->normals.size();
	ret->vNormals = new glm::vec3[numverts];
	int numIndices = ret->indices.size();
	//calculate normals yourself (optional)
	/*for (int i = 0; i < numIndices; i += 3) { // each three indices represent a triangle/ face of cone
		glm::vec3 p1 = ret->vertices[ret->indices[i]];
		glm::vec3 p2 = ret->vertices[ret->indices[i + 1]];
		glm::vec3 p3 = ret->vertices[ret->indices[i + 2]];
		glm::vec3 N = normalize(cross(glm::vec3(p2 - p1), glm::vec3(p3 - p1)));
		ret->vNormals[ret->indices[i]] += N;
		ret->vNormals[ret->indices[i + 1]] += N;
		ret->vNormals[ret->indices[i + 2]] += N;
	}*/
	for (int i = 0; i < ret->normals.size(); i++) ret->vNormals[i] = normalize(ret->normals[i]);

	return ret;
}

#endif //OPENGL_WRAPPER_H
