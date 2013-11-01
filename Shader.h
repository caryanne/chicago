#ifndef _SHADER_H_
#define _SHADER_H_

#include "glfw/glfw3.h"

class Shader {

private:
	GLuint shader;

public:
	Shader(const char *vertexShaderFilename, const char *fragShaderFilename);
	GLuint getAttribLocation(const GLchar* name) { return glGetAttribLocation(shader, name); }
	void use() { glUseProgram(shader); }
	GLuint getProgram() { return shader; }

};

#endif