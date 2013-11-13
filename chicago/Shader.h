#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include "glfw/glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace std;

enum UNIFORM {
	ModelViewProjectionMatrix,
	ModelViewMatrix,
	NormalMatrix,
	EyePosition,
	EyeDirection,
	LightPosition,
	TextureColor,
	TextureNormal,
	Time,
	COUNT
};

class Shader {

private:
	GLuint mShader;
	string mFilename;
	GLuint mUniforms[UNIFORM::COUNT];

public:
	Shader() {}
	Shader(const string& filename);
	void load(const string& filename);
	inline GLuint getAttribLocation(const GLchar* name) { return glGetAttribLocation(mShader, name); }
	inline GLuint getUniformLocation(const GLchar* name) { return glGetUniformLocation(mShader, name); }
	inline GLuint getUniformLocation(UNIFORM uniform) { return mUniforms[uniform]; }
	inline void use() { glUseProgram(mShader); }
	void unload();
	GLuint getProgram() { return mShader; }

	
	void uniformModelViewProjectionMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(mUniforms[UNIFORM::ModelViewProjectionMatrix], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void uniformModelViewMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(mUniforms[UNIFORM::ModelViewMatrix], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void uniformNormalMatrix(glm::mat3 matrix) {
		glUniformMatrix3fv(mUniforms[UNIFORM::NormalMatrix], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void uniformTime(double time) {
		glUniform1f(mUniforms[UNIFORM::Time], time);
	}

	void uniformEyePosition(glm::vec3 position) {
		glUniform3fv(mUniforms[UNIFORM::EyePosition], 1, glm::value_ptr(position));
	}

	void uniformEyeDirection(glm::vec3 direction) {
		glUniform3fv(mUniforms[UNIFORM::EyeDirection], 1, glm::value_ptr(direction));
	}

	void uniformLightPosition(glm::vec4 position) {
		glUniform4fv(mUniforms[UNIFORM::LightPosition], 1, glm::value_ptr(position));
	}

	void uniformTextureColor(GLuint id) {
		glUniform1i(mUniforms[UNIFORM::TextureColor], id);
	}

	void uniformTextureNormal(GLuint id) {
		glUniform1i(mUniforms[UNIFORM::TextureNormal], id);
	}

};

#endif