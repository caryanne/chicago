#ifndef _MODEL_H_
#define _MODEL_H_

#include <gl/glew.h>
#include "glfw/glfw3.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include <map>
#include <string>
#include "Shader.h"
using namespace std;

class Model {

private:
	map<string,unsigned> mTextures;
	vector<tinyobj::shape_t> mData;
	Shader *mShader;
	GLuint vao;
	GLuint vbo;
	GLuint ib;
	GLuint pos;
	GLuint col;

	
public:
	Model(const char* filename, Shader *shader);
	void load(const char* filename, Shader *shader);
	void setShader(Shader *shader) { mShader = shader; }
	void render();


};

#endif