#include "soil\SOIL.h"
#include "Mesh.h"
#include "ShaderManager.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//#define mesh mData[0].mesh

Mesh::Mesh(const string& filename) {
	load(filename);
}

void Mesh::load(const string& filename) {
	double start = glfwGetTime();
	mSubMeshData = vector<SubMeshData>();
	mFilename = filename;
	printf("%.2f:loading mesh %s\n", glfwGetTime(), filename.c_str());
	if(tinyobj::LoadObj(mData, ("media/" + filename).c_str(), "media/") != "")
		return;

	for(unsigned i = 0; i < mData.size(); i++) {
		mSubMeshData.push_back(SubMeshData());
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			printf("%.2f:...loading texture %s\n", glfwGetTime(), mData[i].material.diffuse_texname.c_str());
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(("media/textures/" + mData[i].material.diffuse_texname).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}
		if(mData[i].material.unknown_parameter.find("normalmap") != mData[i].material.unknown_parameter.end()) {
			const string normalmap = mData[i].material.unknown_parameter.find("normalmap")->second;
			printf("%.2f:...loading normalmap %i/%s\n", glfwGetTime(), i, normalmap.c_str());
			mTextures[normalmap] =
				SOIL_load_OGL_texture(string("media/textures/").append(normalmap).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}
		if(mData[i].material.unknown_parameter.find("shader") != mData[i].material.unknown_parameter.end()) {
			printf("%.2f:...loading shader %i/%s\n", glfwGetTime(), i, mData[i].material.unknown_parameter.find("shader")->second.c_str());
			
			mSubMeshData.back().mShader = ShaderManager::getInstance().getShader(mData[i].material.unknown_parameter.find("shader")->second);

		} else {
			printf("%.2f:...using passthrough shader %i\n", glfwGetTime(), i);
			mSubMeshData.back().mShader = ShaderManager::getInstance().getShader("passthrough");
		}

		printf("%.2f:...populating vertex array\n", glfwGetTime());
	
		glGenVertexArrays(1, &mSubMeshData.back().mVAO);

		glBindVertexArray(mSubMeshData.back().mVAO);
		glGenBuffers(1, &mSubMeshData.back().mIB);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSubMeshData.back().mIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mData[i].mesh.indices[0]) * mData[i].mesh.indices.size(), &(mData[i].mesh.indices.front()), GL_STATIC_DRAW);

		glGenBuffers(1, &mSubMeshData.back().mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mSubMeshData.back().mVBO);

		//total size = positions(3), normals(3), tex(2)
		int sizePositions = sizeof(mData[i].mesh.positions[0]) * mData[i].mesh.positions.size();
		int sizeNormals = sizeof(mData[i].mesh.normals[0]) * mData[i].mesh.normals.size();
		int sizeTexCoords = sizeof(mData[i].mesh.texcoords[0]) * mData[i].mesh.texcoords.size();

		glBufferData(GL_ARRAY_BUFFER, sizePositions + sizeNormals + sizeTexCoords, NULL, GL_STATIC_DRAW);// 

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizePositions, &mData[i].mesh.positions[0]);//positions
		glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeNormals, &mData[i].mesh.normals[0]);//normals
		glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, sizeTexCoords, &mData[i].mesh.texcoords[0]);//textures

		GLuint position = mSubMeshData.back().mShader->getAttribLocation("vPosition");
		GLuint normal = mSubMeshData.back().mShader->getAttribLocation("vNormal");
		GLuint texcoord = mSubMeshData.back().mShader->getAttribLocation("vTexCoord");



		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
		glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));
	
		mSubMeshData.back().mShader->use();

		glEnableVertexAttribArray(position);
		glEnableVertexAttribArray(normal);
		glEnableVertexAttribArray(texcoord);

		glBindVertexArray(0);

	}

	printf("%.2f:done loading %s in %.2fms\n", glfwGetTime(), filename.c_str(), (glfwGetTime() - start) * 1000.0);


}

void Mesh::bind(unsigned submesh) {
	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	if(last != mSubMeshData[submesh].mVAO) {
		glBindVertexArray(mSubMeshData[submesh].mVAO);
		mSubMeshData[submesh].mShader->use();
	}
}

void Mesh::draw(unsigned submesh) {
	bind(submesh);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[0].material.diffuse_texname]);
	
	if(mData[0].material.unknown_parameter.find("normalmap") != mData[0].material.unknown_parameter.end()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mTextures[mData[0].material.unknown_parameter.find("normalmap")->second]);
	}
	glDrawElements(GL_TRIANGLES, mData[submesh].mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::reload() {
	printf("%.2f:commence reloading mesh %s\n", glfwGetTime(), mFilename.c_str());
	
	for(int i = 0; i < mSubMeshData.size(); i++) {
		mSubMeshData[i].mShader->unload();

		glDeleteBuffers(1, &mSubMeshData[i].mVBO);
		glDeleteBuffers(1, &mSubMeshData[i].mIB);
		glDeleteVertexArrays(1, &mSubMeshData[i].mVAO);
	}
	load(mFilename);
}