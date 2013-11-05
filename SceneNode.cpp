#include "SceneNode.h"

SceneNode::SceneNode(MoveableObject* obj) {
	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f,1.f,1.f));
	mObj = obj;
}

unsigned SceneNode::addChild(SceneNode *sceneNode) {
	
	if(sceneNode->getParent() == NULL)
	{
		mChildren.push_back(sceneNode);
		sceneNode->setParent(this);
	}
	return mChildren.size();

}