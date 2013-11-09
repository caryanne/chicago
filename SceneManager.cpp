#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"
#include "SceneManager.h"

SceneManager::SceneManager() {
	 mRootNode = SceneNode();
	 mCamera = Camera();
	 mCamera.setFOV(45.f);
	 setScreenRatio(1.77778f);
	 updateMatrices();
}

void SceneManager::updateMatrices() {
	mProjection = glm::perspective( getCamera()->getFOV(), getScreenRatio(), 0.1f, 100.f);
}

void SceneManager::drawNode(SceneNode* sceneNode) {

	for(vector<SceneNode*>::iterator it = sceneNode->getChildren();  it != sceneNode->childrenEnd(); ++it)
		drawNode(*it);
	if(sceneNode->hasRenderable()) {	

		glm::mat4 model = sceneNode->getModelMatrix(); //model
		glm::mat4 mvp = mViewProjection * model; //model-view-projection
		glm::mat4 mv = mView * model; //model-view
		glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv)); //normal
		
		sceneNode->getEntity()->getMesh()->bind(); //bind vao

		//update uniforms


		sceneNode->getEntity()->getMesh()->draw();


	}
/*		
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
	glm::vec3 eye = glm::vec3(5.f * (float)sin(glfwGetTime()), 1.5f, 5.f * (float)cos(glfwGetTime()));
	getCamera()->lookAt(eye, glm::vec3(0,0,0), glm::vec3(0,1,0));
	mView = getCamera()->getView();
	mViewProjection = mProjection * mView;
	drawNode(getRootNode());

}

void SceneManager::reloadNode(SceneNode* sceneNode) {
	
	for(vector<SceneNode*>::iterator it = sceneNode->getChildren();  it != sceneNode->childrenEnd(); ++it)
		reloadNode(*it);
	if(sceneNode->hasRenderable())
		sceneNode->reload();
	
}

void SceneManager::reloadScene() {
	printf("%.2f:commence reloading scene\n", glfwGetTime());
	reloadNode(getRootNode());
	printf("%.2f:done reloading scene\n", glfwGetTime());
}