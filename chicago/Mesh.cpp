#include "soil\SOIL.h"
#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define mesh mData[0].mesh

Mesh::Mesh(const string& filename) {
	load(filename);
}

void Mesh::load(const string& filename) {
	double start = glfwGetTime();

	mFilename = filename;
	printf("%.2f:loading mesh %s\n", glfwGetTime(), filename.c_str());
	tinyobj::LoadObj(mData, ("media/" + filename).c_str(), "media/");
	for(unsigned i = 0; i < mData.size(); i++) {
		if(mTextures.find(mData[i].material.diffuse_texname) == mTextures.end()) {
			printf("%.2f:...loading texture %s\n", glfwGetTime(), mData[i].material.diffuse_texname.c_str());
			mTextures[mData[i].material.diffuse_texname] =
				SOIL_load_OGL_texture(("media/textures/" + mData[i].material.diffuse_texname).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_NTSC_SAFE_RGB);
		}
		if(mData[i].material.unknown_parameter.find("shader") != mData[i].material.unknown_parameter.end()) {
			printf("%.2f:...loading shader %i/%s\n", glfwGetTime(), i, mData[i].material.unknown_parameter.find("shader")->second.c_str());
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

	GLuint position = getShader()->getAttribLocation("vPosition");
	GLuint normal = getShader()->getAttribLocation("vNormal");
	GLuint texcoord = getShader()->getAttribLocation("vTexCoord");

	mUniforms[UNIFORM::ModelViewProjectionMatrix] = getShader()->getUniformLocation("mModelViewProj");
	mUniforms[UNIFORM::ModelViewMatrix] = getShader()->getUniformLocation("mModelView");
	mUniforms[UNIFORM::NormalMatrix] = getShader()->getUniformLocation("mNormalMatrix");
	mUniforms[UNIFORM::EyePosition] = getShader()->getUniformLocation("vEyePosition");
	mUniforms[UNIFORM::EyeDirection] = getShader()->getUniformLocation("vEyeDirection");
	mUniforms[UNIFORM::LightPosition] = getShader()->getUniformLocation("vLightPosition");
	mUniforms[UNIFORM::TextureBase] = getShader()->getUniformLocation("sTexture");
	mUniforms[UNIFORM::Time] = getShader()->getUniformLocation("fTime");

	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizePositions));
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizePositions + sizeNormals));
	
	getShader()->use();

	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(normal);
	glEnableVertexAttribArray(texcoord);

	glBindVertexArray(0);
	
	printf("%.2f:done loading %s in %.2fms\n", glfwGetTime(), filename.c_str(), (glfwGetTime() - start) * 1000.0);


}

void Mesh::bind() {

	getShader()->use();

	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	if(last != mVAO)
		glBindVertexArray(mVAO);
}

void Mesh::draw() {

	
	//update uniforms, bind texture, etc;
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[0].material.diffuse_texname]);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::reload() {
	printf("%.2f:commence reloading mesh %s\n", glfwGetTime(), mFilename.c_str());
	
	for(int i = 0; i < mShaders.size(); i++)
		getShader(i)->unload();

	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIB);
	glDeleteVertexArrays(1, &mVAO);

	load(mFilename);
}