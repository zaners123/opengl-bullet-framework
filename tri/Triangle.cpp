#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <iostream>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS

GLuint  vao;
GLuint  vbo;
GLuint v, f;

GLfloat vertices[] = {
	-.9,-.9,
    .9,-.9,
    .9,.9,
    .9,.9,
    -.9,.9,
    -.9,-.9,
};
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
GLuint initShaders(const char* v_shader, const char* f_shader) {

	GLuint p = glCreateProgram();

	const char * vs = ReadFile(v_shader);
	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vs, NULL);
	free((char*)vs);

	const char * fs = ReadFile(f_shader);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fs, NULL);
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
		exit(1);
	}

	glUseProgram(p);

	return p;

}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
//	glPointSize(15);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
	glFlush();
}
void init() {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint prog = initShaders(
			"../tri/triangle.vs",
			"../tri/triangle.fs");
	//howto set shader values
	glm::vec4 vec = glm::vec4(0.0,0.0,1.0,1.0);
	glUniform4fv(glGetUniformLocation(prog, "fColorIn"), 1, (GLfloat *) &vec[0]);
//	auto s = scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));
	auto s = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(prog, "scale"), 4, GL_FALSE, (GLfloat *) &s[0]);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//background color
	glClearColor(1.0,1.0,0.0,1.0);
}
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Square");

	if (glewInit()) {
		printf("%s\n", "Unable to initialize GLEW ...");
	}

	init();
	/* we can now get data for the specific OpenGL instance we created */
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	
	printf("GLSL Version : %s\n", glslVersion);
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;

}