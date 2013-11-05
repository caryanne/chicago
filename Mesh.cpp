#include "Mesh.h"

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