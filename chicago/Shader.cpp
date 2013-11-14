#include <gl/glew.h>

#include <stdio.h>
#include "Shader.h"

using namespace std;

static char *readFile(const string& filename) {

	FILE *fp = fopen(filename.c_str(), "r");
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *contents = new char[length + 1];
	for(int i = 0; i < length + 1; i++)
		contents[i] = 0;
	fread(contents, 1, length, fp);
	contents[length + 1] = '\0';
	fclose(fp);
	return contents;
}


const char* compileSuccessful(int obj) {
  int status;
  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  return (status == GL_TRUE) ? "OK" : "FAIL";
}

const char* linkSuccessful(int obj) {
  int status;
  glGetProgramiv(obj, GL_LINK_STATUS, &status);
  return (status == GL_TRUE) ? "OK" : "FAIL";
}
GLuint makeVertexShader(const string& filename) {
	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	//printf("%.2f:%s %s\n", glfwGetTime(), filename, compileSuccessful(id));
	return id;
}
GLuint makeFragmentShader(const string& filename) {
	GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	//printf("%.2f:%s %s\n", glfwGetTime(), filename, compileSuccessful(id));
	return id;
}
GLuint makeShaderProgram(GLuint vertexShader, GLuint fragShader) {
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);
	printf("%.2f:shader %i %s [vertex(%s) fragment(%s)]\n", glfwGetTime(), id, linkSuccessful(id), compileSuccessful(vertexShader), compileSuccessful(fragShader));
	return id;
}

Shader::Shader(const string& filename) {
	load(filename);
}

void Shader::load(const string& filename) {
	mShader = makeShaderProgram(makeVertexShader(filename + ".vert"),
								makeFragmentShader(filename + ".frag"));

	mUniforms[UNIFORM::ModelViewProjectionMatrix] = getUniformLocation("mModelViewProj");
	mUniforms[UNIFORM::ModelViewMatrix] = getUniformLocation("mModelView");
	mUniforms[UNIFORM::NormalMatrix] = getUniformLocation("mNormalMatrix");
	mUniforms[UNIFORM::EyePosition] = getUniformLocation("vEyePosition");
	mUniforms[UNIFORM::EyeDirection] = getUniformLocation("vEyeDirection");
	mUniforms[UNIFORM::LightPosition] = getUniformLocation("vLightPosition");
	mUniforms[UNIFORM::TextureColor] = getUniformLocation("sTexture");
	mUniforms[UNIFORM::TextureNormal] = getUniformLocation("sNormal");
	mUniforms[UNIFORM::Time] = getUniformLocation("fTime");
}

void Shader::unload() {
	glDeleteShader(mShader);
	printf("%.2f:deleted shader %i\n", glfwGetTime(), mShader);


}