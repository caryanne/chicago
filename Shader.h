#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include "glfw/glfw3.h"

using namespace std;

class Shader {

private:
	GLuint mShader;

public:
	Shader() {}
	Shader(const string& filename);
	virtual void load(const string& filename);
	inline GLuint getAttribLocation(const GLchar* name) { return glGetAttribLocation(mShader, name); }
	inline GLuint getUniformLocation(const GLchar* name) { return glGetUniformLocation(mShader, name); }
	inline void use() { glUseProgram(mShader); }
	GLuint getProgram() { return mShader; }

};

#endif