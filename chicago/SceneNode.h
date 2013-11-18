#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include "bullet\btBulletDynamicsCommon.h"
#include "Mesh.h"

using namespace std;

class SceneNode {

private:
	vector<SceneNode*> mChildren;
	SceneNode* mParent;
	Mesh* mEntity;
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	btVector3 mInertia;
	btCollisionShape* mCollisionShape;
	btDefaultMotionState* mMotionState;
	btRigidBody* mRigidBody;

public:
	SceneNode();
	SceneNode(Mesh* entity, bool noPhysics = false);
	~SceneNode();
	unsigned addChild(SceneNode* sceneNode);
	
	bool hasRenderable() { return mEntity != NULL; }

	vector<SceneNode*>::iterator getChildren() { return mChildren.begin(); }
	vector<SceneNode*>::iterator childrenEnd() { return mChildren.end(); }
	void attach(Mesh* entity) { mEntity = entity; }
	Mesh* getEntity() { return mEntity; }
	SceneNode* getParent() { return mParent; }
	void setParent(SceneNode* sceneNode) { mParent = sceneNode; }
	void setPosition(glm::vec3 position);

	void setRotation(glm::quat rotation);
	void setRotation(glm::vec3 rotation);

	glm::vec3 getPosition() { return mPosition; }

	void setScale(glm::vec3 scale) { mScale = scale; }
	void reload();
	void bind(unsigned submesh);
	void draw(unsigned submesh);

	glm::mat4 getModelMatrix();

	btRigidBody* getRigidBody() { return mRigidBody; }

	//void detach();
	//void removeChild();

};

#endif