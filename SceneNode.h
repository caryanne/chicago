#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"

using namespace std;

class SceneNode {

private:
	vector<SceneNode> mChildren;
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

public:
	SceneNode();
	void addChild();
	void setPosition(glm::vec3 position) { mPosition = position; }
	void setRotation(glm::quat rotation) { mRotation = rotation; }
	void setScale(glm::vec3 scale) { mScale = scale; }

};

#endif