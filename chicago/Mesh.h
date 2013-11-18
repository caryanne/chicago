#ifndef _MESH_H_
#define _MESH_H_

#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include <map>
#include <string>
#include "Shader.h"

using namespace std;


struct SubMeshData {
	GLuint mVAO;
	GLuint mVBO;
	GLuint mIB;
	Shader *mShader;
};

class Mesh {

private:
	map<string, unsigned> mTextures;

	vector<tinyobj::shape_t> mData;
	string mFilename;

	vector<SubMeshData> mSubMeshData;

	float mMass;


public:
	Mesh() {
	
	};
	Mesh(const string& filename);
	void load(const string& filename);

	unsigned subMeshCount() { return mData.size(); }
	Shader* subMeshShader(unsigned submesh) { return mSubMeshData[submesh].mShader; }
	void bind(unsigned submesh);
	glm::vec3 halfExtents(int submesh = -1);
	void draw(unsigned submesh);
	void reload();
	float getMass() { return mMass; }

};

#endif