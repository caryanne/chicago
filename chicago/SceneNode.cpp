#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"
#include "SceneNode.h"

SceneNode::SceneNode(Mesh *entity) {
	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f,1.f,1.f));
	mEntity = entity;
	mParent = NULL; 
}

SceneNode::SceneNode() {

	mEntity = NULL;
	mParent = NULL; 

	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f, 1.f, 1.f));
}

SceneNode::~SceneNode() {

}

unsigned SceneNode::addChild(SceneNode *sceneNode) {
	
	if(sceneNode->getParent() == NULL)
	{
		mChildren.push_back(sceneNode);
		sceneNode->setParent(this);
	}
	return mChildren.size();

}

void SceneNode::reload() {
	
	if(mEntity != NULL)
		mEntity->reload();
}
void SceneNode::bind(unsigned submesh) {
	if(hasRenderable())
		mEntity->bind(submesh);

}

void SceneNode::draw(unsigned submesh) {
	if(hasRenderable())
		mEntity->draw(submesh);
}

glm::mat4 SceneNode::getModelMatrix() {
	glm::mat4 scale = glm::scale(glm::mat4(1.f), mScale);
	glm::mat4 translation = glm::translate(glm::mat4(1.f), mPosition);
	glm::mat4 rotation = glm::toMat4(mRotation);
	return scale * translation * rotation;
}

void SceneNode::setPosition(glm::vec3 position) { 
	mPosition = position;
}

void SceneNode::setRotation(glm::quat rotation) { 
	mRotation = rotation;
}

void SceneNode::setRotation(glm::vec3 rotation) { 
	mRotation = glm::quat(rotation);
}