#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include "MoveableObject.h"

using namespace std;

class SceneNode {

private:
	vector<SceneNode*> mChildren;
	SceneNode* mParent;
	MoveableObject* mObj;
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

public:
	SceneNode();
	virtual unsigned addChild(SceneNode* sceneNode);
	virtual void attach(MoveableObject* obj) { mObj = obj; }
	virtual SceneNode* getParent() { return mParent; }
	virtual void setParent(SceneNode* sceneNode) { mParent = sceneNode; }
	virtual void setPosition(glm::vec3 position) { mPosition = position; }
	virtual void setRotation(glm::quat rotation) { mRotation = rotation; }
	virtual void setScale(glm::vec3 scale) { mScale = scale; }
	virtual void detach();
	virtual void removeChild();

};

#endif