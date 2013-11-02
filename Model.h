#ifndef _MODEL_H_
#define _MODEL_H_

#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include <map>
#include <string>
#include "Shader.h"
using namespace std;



class Model {

private:
	map<string,unsigned> mTextures;
	vector<tinyobj::shape_t> mData;
	Shader *mShader;

	GLuint mVAO;
	GLuint mVBO;
	GLuint mIB;
	
	GLuint mMVP;
	GLuint mMV;
	GLuint mNM;
	GLuint mEye;
	GLuint mTexture;

	glm::vec3 mPosition;
	glm::quat mRotation;

	
public:
	Model(const char* filename, Shader *shader);
	void load(const char* filename, Shader *shader);
	void setShader(Shader *shader) { mShader = shader; }
	void render(glm::vec3 eye, glm::mat4 view, glm::mat4 viewProjection);
	void setPosition(glm::vec3 position) { mPosition = position; }
	void setRotation(glm::quat rotation) { mRotation = rotation; }

};

#endif