#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include "Entity.h"

using namespace std;

class SceneNode {

private:
	vector<SceneNode*> mChildren;
	SceneNode* mParent;
	Entity* mEntity;
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

public:
	SceneNode();
	SceneNode(Entity* entity);
	unsigned addChild(SceneNode* sceneNode);
	
	bool hasRenderable() { return mEntity != NULL ? mEntity->getMesh() != NULL : false; }

	vector<SceneNode*>::iterator getChildren() { return mChildren.begin(); }
	vector<SceneNode*>::iterator childrenEnd() { return mChildren.end(); }
	void attach(Entity* entity) { mEntity = entity; }
	Entity* getEntity() { return mEntity; }
	SceneNode* getParent() { return mParent; }
	void setParent(SceneNode* sceneNode) { mParent = sceneNode; }
	void setPosition(glm::vec3 position) { mPosition = position; }

	void setRotation(glm::quat rotation) { mRotation = rotation; }
	void setRotation(glm::vec3 rotation) { mRotation = glm::quat(rotation); }

	glm::vec3 getPosition() { return mPosition; }

	void setScale(glm::vec3 scale) { mScale = scale; }
	void reload();
	void bind();
	void draw();

	glm::mat4 getModelMatrix();


	//void detach();
	//void removeChild();

};

#endif