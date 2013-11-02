#include "Model.h"
#include "soil\SOIL.h"
#define mesh mData[0].mesh
Model::Model(const char* filename, Shader *shader) {
	load(filename, shader);
}

void Model::load(const char* filename, Shader *shader) {
	/*printf("loading %s:%s\n", filename, tinyobj::LoadObj(mData, filename).c_str());
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(mData[i].material.diffuse_texname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
			printf("loading %s..\n", mData[i].material.diffuse_texname.c_str());
		}
	}
	printf("model info:%s\n..vertex count:%i\n..index count:%i\n", mData[0].name.c_str(), mesh.positions.size() / 3, mesh.indices.size());
	*/
	/*float positions[] = {1.0, -1.0, -1.0,
1.0, -1.0, 1.0,
-1.0, -1.0, 1.0,
-1.0, -1.0, -1.0,
1.0, 1.0, -1.0,
1.0 ,1.0, 1.0,
-1.0, 1.0, 1.0,
-1.0, 1.0, -1.0 };

	int indices[] = { 1, 2, 3,
 1, 3, 4,
 5, 8, 7,
 5, 7, 6,
 1, 5, 6,
 1, 6, 2,
 2, 6, 7,
 2, 7, 3,
 3, 7, 8,
 3, 8, 4,
 5, 1, 4,
 5, 4, 8};*/
 float positions[] = {0.0,0.0,0.0,1.0,
					0.0,1.0,0.0,1.0,
					1.0,0.0,0.0,1.0};
 GLubyte indices[] = {0,1,2};



	
	/*glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0]) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	//total size = positions(3), normals(3), tex(2)
	//int sizePositions = sizeof(mesh.positions[0]) * mesh.positions.size();
	//int sizeNormals = sizeof(mesh.normals[0]) * mesh.normals.size();
	//int sizeTexCoords = sizeof(mesh.texcoords[0]) * mesh.texcoords.size();
	//glBufferData(GL_ARRAY_BUFFER, sizePositions, NULL, GL_STATIC_DRAW);// + sizeNormals + sizeTexCoords
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), NULL, GL_STATIC_DRAW);// + sizeNormals + sizeTexCoords
	//offset, size, data	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);//positions
	//glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeNormals, &mesh.normals[0]);//normals
	//glBufferSubData(GL_ARRAY_BUFFER, sizePositions + sizeNormals, sizeTexCoords, &mesh.texcoords[0]);//textures

	mShader = shader;

	GLuint position = mShader->getAttribLocation("vPosition");
	GLuint normal = mShader->getAttribLocation("vNormal");
	GLuint texcoord = mShader->getAttribLocation("vTexCoord");

	glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizePositions);
	//glVertexAttribPointer(texcoord, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizePositions + sizeNormals));
	
	mShader->use();

	glEnableVertexAttribArray(position);
	//glEnableVertexAttribArray(normal);
	//glEnableVertexAttribArray(texcoord);

	//glBindVertexArray(0);
}

void Model::render() {
	glBindVertexArray(vao);
	mShader->use();
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glDrawArrays(GL_TRIANGLES,0,3);
	//glBindVertexArray(0);
}