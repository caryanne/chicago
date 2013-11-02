#include "Model.h"
#include "soil/SOIL.h"
#include <iostream>
#include "glm/glm.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define mesh mData[0].mesh

Model::Model(const char* filename, Shader *shader) {
	load(filename, shader);
}

void Model::load(const char* filename, Shader *shader) {
	printf("loading %s:%s\n", filename, tinyobj::LoadObj(mData, filename).c_str());
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(mData[i].material.diffuse_texname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
			printf("loading %s..\n", mData[i].material.diffuse_texname.c_str());
		}
	}


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0]) * mesh.indices.size(), &(mesh.indices.front()), GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//total size = positions(3), normals(3), tex(2)
	int sizePositions = sizeof(mesh.positions[0]) * mesh.positions.size();
	int sizeNormals = sizeof(mesh.normals[0]) * mesh.normals.size();
	//int sizeTexCoords = sizeof(mesh.texcoords[0]) * mesh.texcoords.size();

	glBufferData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, NULL, GL_STATIC_DRAW);// 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);// + sizeNormals + sizeTexCoords
	//offset, size, data	
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);//positions
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);//colors
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizePositions, &mesh.positions[0]);//positions
	glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeNormals, &mesh.normals[0]);//normals
	//glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, sizeTexCoords, &mesh.texcoords[0]);printf("coords\n");//textures

	mShader = shader;

	GLuint position = mShader->getAttribLocation("vPosition");
	//GLuint color = mShader->getAttribLocation("vColor");
	GLuint normal = mShader->getAttribLocation("vNormal");
	//GLuint texcoord = mShader->getAttribLocation("vTexCoord");
	//printf("position location:%i//normal location:%i\n",position, normal);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(positions)));
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
	//glVertexAttribPointer(texcoord, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));

	mShader->use();

	glEnableVertexAttribArray(position);
	//glEnableVertexAttribArray(color);
	glEnableVertexAttribArray(normal);
	//glEnableVertexAttribArray(texcoord);

	glBindVertexArray(0);
	printf("done\n");
}

void Model::render() {
	
	glBindVertexArray(vao);
	mShader->use();
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

	//glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//glDrawArrays(GL_TRIANGLES,0,3);
	glBindVertexArray(0);
}