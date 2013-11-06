#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "SceneManager.h"

void SceneManager::drawNode(SceneNode* sceneNode) {

/*	glm::mat4 scale = glm::scale(glm::mat4(1.f), mScale);
	glm::mat4 translation = glm::translate(glm::mat4(1.f), mPosition);
	glm::mat4 rotation = glm::toMat4(mRotation);
	glm::mat4 model =  scale * translation * rotation;  
	glm::mat4 mvp = viewProjection * model;
	glm::mat4 mv = view * model;
	glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv));

	sceneNode->getObj()->bind();
	
	mShaders[0].use();

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix3fv(uniformNM, 1, GL_FALSE, glm::value_ptr(nm));

	glUniform1f(mShaders[0].getUniformLocation("time"),glfwGetTime());
	glUniform3fv(uniformEye, 1, glm::value_ptr(eye));
	
	glUniform1i(uniformTexBase, 0);
	glBindTexture(GL_TEXTURE_2D, mTextures[mData[0].material.diffuse_texname]);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	*/
	//while has children, iterate though


}

void SceneManager::drawScene() {
	//figure out matrices
	//iterate through all children
	// bind vao
	// update uniforms
	// draw object
	
	drawNode(getRootNode());

}