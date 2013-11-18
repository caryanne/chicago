#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "SceneManager.h"

SceneManager::SceneManager() {
	 //mRootNode = SceneNode();
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
		glm::vec4 lightPos = mView * mLightPos;
		
		for(unsigned i = 0; i < sceneNode->getEntity()->subMeshCount(); i++) {
			sceneNode->bind(i);
			Mesh* mesh = sceneNode->getEntity();
			mesh->subMeshShader(i)->uniformModelViewProjectionMatrix(mvp);
			mesh->subMeshShader(i)->uniformModelViewMatrix(mv);
			mesh->subMeshShader(i)->uniformNormalMatrix(nm);
			mesh->subMeshShader(i)->uniformTime(glfwGetTime());
			mesh->subMeshShader(i)->uniformEyePosition(mCamera.getPosition());
			mesh->subMeshShader(i)->uniformEyeDirection(mCamera.getDirection());
			mesh->subMeshShader(i)->uniformLightPosition(lightPos);
			mesh->subMeshShader(i)->uniformTextureColor(0);
			mesh->subMeshShader(i)->uniformTextureNormal(1);
			sceneNode->draw(i);
		}

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