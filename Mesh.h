#ifndef _MESH_H_
#define _MESH_H_

#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "tinyobjloader\tiny_obj_loader.h"

#include <map>
#include <string>
#include "Shader.h"


using namespace std;

class Mesh {

private:
	map<string, unsigned> mTextures;
	map<unsigned, Shader> mShaders;
	vector<tinyobj::shape_t> mData;
	string mFilename;

	GLuint mVAO;
	GLuint mVBO;
	GLuint mIB;
	
	GLuint uniformMVP;
	GLuint uniformMV;
	GLuint uniformNM;
	GLuint uniformEye;
	GLuint uniformTexBase;
	
public:
	Mesh(const string& filename);
	void load(const string& filename);
	void setShader(unsigned index, Shader shader) { mShaders[index] = shader; }
	Shader* getShader(unsigned index = 0) { return &mShaders[index]; }

	void bind();
	void draw();
	void reload();

	GLuint getVertexBuffer() { return mVBO; }
	GLuint getVertexArray() { return mVAO; }
	GLuint getIndexBuffer() { return mIB; }

};

#endif