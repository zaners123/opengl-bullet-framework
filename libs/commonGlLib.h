#ifndef OPENGL_COMMONGLLIB_H
#define OPENGL_COMMONGLLIB_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

char* ReadFile(const char* filename) {
	FILE* infile;
#ifdef WIN32
	fopen_s(&infile, filename, "rb");
#else
	infile = fopen(filename, "rb");
#endif
	if (!infile) {
		printf("Unable to open file %s\n", filename);
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	char* source = (char*)malloc(len + 1);
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	return (source);
}

/*************************************************************/
/************************************/
GLuint loadTexture(const char* filename) {

	ILboolean success = false;

	ILuint imageID = 0;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
//	std::cout<<"Gen image ID:"<<imageID<<" for "<<filename<<std::endl;
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	success = ilLoadImage(filename);

	if (!success) {
		printf("Couldn't load the following texture file: %s\n", filename);
		// The operation was not successful hence free image and texture
		ilDeleteImages(1, &imageID);
		return 0;
	} else {
		std::cout<<"I was able to load image"<<filename<<'\n';
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
			  ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),
			  0,GL_RGBA,GL_UNSIGNED_BYTE,ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Because we have already copied image data into texture data we can release memory used by image.
	ilDeleteImages(1, &imageID);
	return tid;
}
/*************************************************************/

/**
 * Returns Opengl program ID, given a vertex shader and fragment shader
 * @todo use map so this is only initialized once per shader
 * */
GLuint initShaders(const char* v_shader, const char* f_shader) {
	GLuint p = glCreateProgram();
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);

	free((char*)vs);
	free((char*)fs);

	glCompileShader(v);

	GLint compiled;

	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		char* log = (char*)malloc(len + 1);

		glGetShaderInfoLog(v, len, &len, log);

		printf("Vertex Shader compilation failed: %s\n", log);

		free(log);

	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len + 1);
		glGetShaderInfoLog(f, len, &len, log);
		printf("Vertex Shader compilation failed: %s\n", log);
		free(log);
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);
	GLint linked;

	glGetProgramiv(p, GL_LINK_STATUS, &linked);

	if (!linked) {

		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len + 1);
		glGetProgramInfoLog(p, len, &len, log);
		printf("Shader linking failed: %s\n", log);
		free(log);
	}

	glUseProgram(p);

	return p;

}

#endif //OPENGL_COMMONGLLIB_H
