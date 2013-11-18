#include "soil\SOIL.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//#define mesh mData[0].mesh

Mesh::Mesh(const string& filename) {
	load(filename);
}

void Mesh::load(const string& filename) {
	double start = glfwGetTime();
	mSubMeshData = vector<SubMeshData>();
	mFilename = filename;
	mMass = 0.f;
	printf("%.2f:loading mesh %s\n", glfwGetTime(), filename.c_str());
	if(tinyobj::LoadObj(mData, ("media/" + filename).c_str(), "media/") != "")
		return;

	for(unsigned i = 0; i < mData.size(); i++) {
		mSubMeshData.push_back(SubMeshData());
		//glm::vec3 hx = halfExtents(i);
		//cout << hx.x << ',' << hx.y << ',' << hx.z << '\n';		

		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			printf("%.2f:...loading texture %s\n", glfwGetTime(), mData[i].material.diffuse_texname.c_str());
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(("media/textures/" + mData[i].material.diffuse_texname).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}

		if(mTextures.find(mData[i].material.normal_texname) == mTextures.end()) {
			printf("%.2f:...loading normal map %s\n", glfwGetTime(), mData[i].material.normal_texname.c_str());
			mTextures[mData[i].material.normal_texname] =
				SOIL_load_OGL_texture(("media/textures/" + mData[i].material.normal_texname).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}

		if(mData[i].material.unknown_parameter.find("shader") != mData[i].material.unknown_parameter.end()) {
			printf("%.2f:...selecting shader %i/%s\n", glfwGetTime(), i, mData[i].material.unknown_parameter.find("shader")->second.c_str());
			mSubMeshData.back().mShader = ShaderManager::getInstance().getShader(mData[i].material.unknown_parameter.find("shader")->second);
		} else {
			printf("%.2f:...using passthrough shader %i\n", glfwGetTime(), i);
			mSubMeshData.back().mShader = ShaderManager::getInstance().getShader("passthrough");
		}

		if(mData[i].material.unknown_parameter.find("mass") != mData[i].material.unknown_parameter.end()) {
			float mass = atof(mData[i].material.unknown_parameter["mass"].c_str());
			printf("%.2f:...mass %.2f\n", glfwGetTime(), mass);
			mMass = mass;
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
		int sizeTangents = sizeof(mData[i].mesh.tangents[0]) * mData[i].mesh.tangents.size();
		int sizeBinormals = 0;//sizeof(mData[i].mesh.binormals[0]) * mData[i].mesh.binormals.size();

		glBufferData(GL_ARRAY_BUFFER, sizePositions + sizeNormals + sizeTexCoords + sizeBinormals + sizeTangents, NULL, GL_STATIC_DRAW);// 

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizePositions, &mData[i].mesh.positions[0]);//positions
		glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeNormals, &mData[i].mesh.normals[0]);//normals
		glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, sizeTexCoords, &mData[i].mesh.texcoords[0]);//textures
		glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals + sizeTexCoords, sizeTangents, &mData[i].mesh.tangents[0]); //tangents
		//glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals + sizeTexCoords + sizeTangents, sizeBinormals, &mData[i].mesh.binormals[0]); //binormals


		GLuint position = mSubMeshData.back().mShader->getAttribLocation("vPosition");
		GLuint normal = mSubMeshData.back().mShader->getAttribLocation("vNormal");
		GLuint texcoord = mSubMeshData.back().mShader->getAttribLocation("vTexCoord");
		GLuint tangent = mSubMeshData.back().mShader->getAttribLocation("vTangent");
		//GLuint binormal = mSubMeshData.back().mShader->getAttribLocation("vBinormal");


		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
		glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));
		glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions + sizeNormals + sizeTexCoords));
		//glVertexAttribPointer(binormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions + sizeNormals + sizeTexCoords + sizeTangents));
	
		mSubMeshData.back().mShader->use();

		glEnableVertexAttribArray(position);
		glEnableVertexAttribArray(normal);
		glEnableVertexAttribArray(texcoord);
		glEnableVertexAttribArray(tangent);
		//glEnableVertexAttribArray(binormal);

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
	mSubMeshData[submesh].mShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[submesh].material.diffuse_texname]);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[submesh].material.normal_texname]);

	glDrawElements(GL_TRIANGLES, mData[submesh].mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
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

glm::vec3 Mesh::halfExtents(int submesh) {
	if(submesh > -1) { 
		float minX = mData[submesh].mesh.positions[0], maxX = mData[submesh].mesh.positions[0];
		float minY = mData[submesh].mesh.positions[1], maxY = mData[submesh].mesh.positions[1];
		float minZ = mData[submesh].mesh.positions[2], maxZ = mData[submesh].mesh.positions[2];

		for(unsigned i = 0; i < mData[submesh].mesh.positions.size(); i += 3) {
			minX = min(minX, mData[submesh].mesh.positions[i]);
			minY = min(minX, mData[submesh].mesh.positions[i+1]);
			minZ = min(minX, mData[submesh].mesh.positions[i+2]);
			maxX = max(maxX, mData[submesh].mesh.positions[i]);
			maxY = max(maxX, mData[submesh].mesh.positions[i+1]);
			maxZ = max(maxX, mData[submesh].mesh.positions[i+2]);
		}
		return glm::vec3((maxX - minX) / 2.f,
						(maxY - minY) / 2.f,
						(maxZ - minZ) / 2.f);
	}
	float minX = mData[0].mesh.positions[0], maxX = mData[0].mesh.positions[0];
	float minY = mData[0].mesh.positions[1], maxY = mData[0].mesh.positions[1];
	float minZ = mData[0].mesh.positions[2], maxZ = mData[0].mesh.positions[2];
	for(unsigned s = 0; s < mData.size(); s++) {
		for(unsigned i = 0; i < mData[s].mesh.positions.size(); i += 3) {
			minX = min(minX, mData[s].mesh.positions[i]);
			minY = min(minX, mData[s].mesh.positions[i+1]);
			minZ = min(minX, mData[s].mesh.positions[i+2]);
			maxX = max(maxX, mData[s].mesh.positions[i]);
			maxY = max(maxX, mData[s].mesh.positions[i+1]);
			maxZ = max(maxX, mData[s].mesh.positions[i+2]);
		}
	}
	return glm::vec3((maxX - minX) / 2.f,
					(maxY - minY) / 2.f,
					(maxZ - minZ) / 2.f);

}


