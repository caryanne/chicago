#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
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
	if( sceneNode->hasRenderable()) {

		glm::mat4 model = sceneNode->getParent()->getModelMatrix() * sceneNode->getModelMatrix(); //model
		glm::mat4 mvp = mViewProjection * model; //model-view-projection
		glm::mat4 mv = mView * model; //model-view
		glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv)); //normal
	
		sceneNode->bind();

		glUniformMatrix4fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::ModelViewProjectionMatrix),
							1, GL_FALSE, glm::value_ptr(mvp));

		glUniformMatrix4fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::ModelViewMatrix),
							1, GL_FALSE, glm::value_ptr(mv));

		glUniformMatrix3fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::NormalMatrix),
							1, GL_FALSE, glm::value_ptr(nm));

		glUniform1f(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::Time), glfwGetTime());

		glUniform3fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::EyePosition), 1, glm::value_ptr(mCamera.getPosition()));

		glUniform3fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::EyeDirection), 1, glm::value_ptr(mCamera.getDirection()));
		
		glm::vec3 camPos = mCamera.getPosition();
		glm::vec4 lightPos = mView * glm::vec4(10*sin(glfwGetTime()), 4 + sin(glfwGetTime() * 5.0), 0.0, 1.0);
		
		glUniform4fv(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::LightPosition), 1, glm::value_ptr(lightPos));


		glUniform1i(sceneNode->getEntity()->getMesh()->getUniform(UNIFORM::TextureBase), 0);


		sceneNode->draw();


	}

}

void SceneManager::drawScene() {
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