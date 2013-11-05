#include "SceneNode.h"

unsigned SceneNode::addChild(SceneNode *sceneNode) {
	
	if(sceneNode->getParent() == NULL)
	{
		mChildren.push_back(sceneNode);
		sceneNode->setParent(this);
	}
	return mChildren.size();

}