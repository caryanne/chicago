#include "soil\SOIL.h"
#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define mesh mData[0].mesh

Mesh::Mesh(const string& filename) {
	load(filename);
}

void Mesh::load(const string& filename) {
	double start = glfwGetTime();
	printf("%.2f:loading mesh %s\n", glfwGetTime(), filename.c_str());
	tinyobj::LoadObj(mData, ("media/" + filename).c_str(), "media/");
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			printf("%.2f:...loading texture %s\n", glfwGetTime(), mData[i].material.diffuse_texname.c_str());
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(("media/textures/" + mData[i].material.diffuse_texname).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}
		if(mData[i].material.unknown_parameter.find("shader") != mData[i].material.unknown_parameter.end()) {
			printf("%.2f:...loading shader %s\n", glfwGetTime(), mData[i].material.unknown_parameter.find("shader")->second.c_str());
			mShaders[i] = Shader(("media/shaders/" + mData[i].material.unknown_parameter.find("shader")->second).c_str());
		}
	}
	printf("%.2f:...populating vertex array\n", glfwGetTime());
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0]) * mesh.indices.size(), &(mesh.indices.front()), GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	//total size = positions(3), normals(3), tex(2)
	int sizePositions = sizeof(mesh.positions[0]) * mesh.positions.size();
	int sizeNormals = sizeof(mesh.normals[0]) * mesh.normals.size();
	int sizeTexCoords = sizeof(mesh.texcoords[0]) * mesh.texcoords.size();

	glBufferData(GL_ARRAY_BUFFER, sizePositions + sizeNormals + sizeTexCoords, NULL, GL_STATIC_DRAW);// 

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizePositions, &mesh.positions[0]);//positions
	glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeNormals, &mesh.normals[0]);//normals
	glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, sizeTexCoords, &mesh.texcoords[0]);//textures

	GLuint position = mShaders[0].getAttribLocation("vPosition");
	GLuint normal = mShaders[0].getAttribLocation("vNormal");
	GLuint texcoord = mShaders[0].getAttribLocation("vTexCoord");

	uniformMVP = mShaders[0].getUniformLocation("mModelViewProj");
	uniformMV = mShaders[0].getUniformLocation("mModelView");
	uniformNM = mShaders[0].getUniformLocation("mNormalMatrix");
	uniformEye = mShaders[0].getUniformLocation("vEye");
	uniformTexBase = mShaders[0].getUniformLocation("sTexture");


	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));
	
	mShaders[0].use();

	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(normal);
	glEnableVertexAttribArray(texcoord);

	glBindVertexArray(0);
	
	printf("%.2f:done loading %s in %.2fms\n", glfwGetTime(), filename.c_str(), (glfwGetTime() - start) * 1000.0);


}

void Mesh::bind() {
	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	if(last != mVAO)
		glBindVertexArray(mVAO);
}

void Mesh::draw() {
	bind();
	
	//update uniforms, bind texture, etc;

	//glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}