#include "Model.h"
#include "soil\SOIL.h"

Model::Model(const char* filename, Shader *shader) {
	load(filename, shader);
}

void Model::load(const char* filename, Shader *shader) {
	tinyobj::LoadObj(mData, filename);
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(mData[i].material.diffuse_texname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}
	}
	
	
	
	GLfloat verts[] = {-2.5f, 0.f, 0.f,
						0.f, 2.f, 0.f,
						2.5f, 0.f, 0.f,
						-2.5f, 4.f, 0.f,
						2.5f, 4.f, 0.f };

	GLfloat color[] = { 1.f, 0.f, 0.f, 1.f,
						0.f, 1.f, 0.f, 1.f,
						1.f, 0.f, 0.f, 1.f,
						0.f, 0.f, 1.f, 1.f,
						0.f, 0.f, 1.f, 1.f};

	GLfloat normal[] = {0.f, 0.f, 1.f,
						0.f, 0.f, 1.f,
						0.f, 0.f, 1.f,
						0.f, 0.f, 1.f,
						0.f, 0.f, 1.f};

	GLubyte indices[] = { 0, 1, 2,
						  3, 1, 4 };
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mData[0].mesh.indices[0] * mData[0].mesh.indices.size()), &mData[0].mesh.indices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 10 * 5, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (3*5) * sizeof(GLfloat), verts);
	glBufferSubData(GL_ARRAY_BUFFER, (3*5) * sizeof(GLfloat), (4*5) * sizeof(GLfloat), color);
	glBufferSubData(GL_ARRAY_BUFFER, (3*5 + 5*4) * sizeof(GLfloat), (3*4) * sizeof(GLfloat), normal);


	
	mShader = shader;
	GLuint pos = mShader->getAttribLocation("vpos");
	GLuint col = mShader->getAttribLocation("vcolor");
	GLuint nor = mShader->getAttribLocation("vnormal");

	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(col, 4, GL_FLOAT, GL_FALSE, 0, ((char*)NULL + ((3*5) * sizeof(GLfloat))));
	glVertexAttribPointer(nor, 3, GL_FLOAT, GL_FALSE, 0, ((char*)NULL + ((3*5+5*4) * sizeof(GLfloat))));
		
	glEnableVertexAttribArray(pos);
	glEnableVertexAttribArray(col);
	glEnableVertexAttribArray(nor);

	glBindVertexArray(0);


}

void Model::render() {
	glBindVertexArray(vao);
	mShader->use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
	glBindVertexArray(0);
}