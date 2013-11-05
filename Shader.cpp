#include <gl/glew.h>

#include <stdio.h>
#include "Shader.h"

using namespace std;

static char *readFile(const char *filename) {
	FILE *fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *contents = new char[length + 1];
	for(int i = 0; i < length + 1; i++)
		contents[i] = 0;
	fread(contents, 1, length, fp);
	contents[length + 1] = '\0';
	fclose(fp);
	return contents;
}


const char* compileSuccessful(int obj) {
  int status;
  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  return (status == GL_TRUE) ? "OK" : "FAIL";
}

const char* linkSuccessful(int obj) {
  int status;
  glGetProgramiv(obj, GL_LINK_STATUS, &status);
  return (status == GL_TRUE) ? "OK" : "FAIL";
}
GLuint makeVertexShader(const char *filename) {
	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	//printf("%.2f:%s %s\n", glfwGetTime(), filename, compileSuccessful(id));
	return id;
}
GLuint makeFragmentShader(const char *filename) {
	GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	//printf("%.2f:%s %s\n", glfwGetTime(), filename, compileSuccessful(id));
	return id;
}
GLuint makeShaderProgram(GLuint vertexShader, GLuint fragShader) {
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);
	printf("%.2f:shader %i %s [vertex(%s) fragment(%s)]\n", glfwGetTime(), id, linkSuccessful(id), compileSuccessful(vertexShader), compileSuccessful(fragShader));
	return id;
}

Shader::Shader(const char *vertexShaderFilename, const char *fragShaderFilename) {
	shader = makeShaderProgram(makeVertexShader(vertexShaderFilename), makeFragmentShader(fragShaderFilename));
}