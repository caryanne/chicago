#include "Model.h"
#include "soil\SOIL.h"
#include <iostream>
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define mesh mData[0].mesh

Model::Model(string filename, Shader *shader) {
	load(filename, shader);
}

void Model::load(string filename, Shader *shader) {
	tinyobj::LoadObj(mData, filename.c_str());
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(mData[i].material.diffuse_texname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
			printf("loading texture %s..\n", mData[i].material.diffuse_texname.c_str());
		}
	}
	
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

	mShader = shader;

	GLuint position = mShader->getAttribLocation("vPosition");
	GLuint normal = mShader->getAttribLocation("vNormal");
	GLuint texcoord = mShader->getAttribLocation("vTexCoord");

	uniformMVP = mShader->getUniformLocation("mModelViewProj");
	uniformMV = mShader->getUniformLocation("mModelView");
	uniformNM = mShader->getUniformLocation("mNormalMatrix");
	uniformEye = mShader->getUniformLocation("vEye");
	uniformTexBase = mShader->getUniformLocation("sTexture");


	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));
	
	mShader->use();

	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(normal);
	glEnableVertexAttribArray(texcoord);

	glBindVertexArray(0);
	
	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f, 1.f, 1.f));

	printf("done\n");
}

void Model::render(glm::vec3 eye, glm::mat4 view, glm::mat4 viewProjection ) {
	glm::mat4 scale = glm::scale(glm::mat4(1.f), mScale);
	glm::mat4 translation = glm::translate(glm::mat4(1.f), mPosition);
	glm::mat4 rotation = glm::toMat4(mRotation);
	glm::mat4 model =  scale * translation * rotation;  
	glm::mat4 mvp = viewProjection * model;
	glm::mat4 mv = view * model;
	glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv));

	glBindVertexArray(mVAO);
	mShader->use();

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix3fv(uniformNM, 1, GL_FALSE, glm::value_ptr(nm));

	glUniform1f(mShader->getUniformLocation("time"),glfwGetTime());
	glUniform3fv(uniformEye, 1, glm::value_ptr(eye));
	
	glUniform1i(uniformTexBase, 0);
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[0].material.diffuse_texname]);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}