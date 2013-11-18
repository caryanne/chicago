#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"
#include "SceneNode.h"

SceneNode::SceneNode(Mesh *entity, bool noPhysics) {
	mCollisionShape = NULL;
	mMotionState = NULL;
	mRigidBody = NULL;
	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f,1.f,1.f));
	mEntity = entity;
	mParent = NULL; 
	if(hasRenderable() && !noPhysics) {
		glm::vec3 size = mEntity->halfExtents();
		mCollisionShape = new btBoxShape(btVector3(size.x, size.y, size.z));
		mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		mCollisionShape->calculateLocalInertia(mEntity->getMass(), mInertia);
		btRigidBody::btRigidBodyConstructionInfo
			mRigidBodyCI(0, mMotionState, mCollisionShape, mInertia);
		mRigidBody = new btRigidBody(mRigidBodyCI);
	}
}

SceneNode::SceneNode() {

	mEntity = NULL;
	mParent = NULL; 
	mCollisionShape = NULL;
	mMotionState = NULL;
	mRigidBody = NULL;
	setPosition(glm::vec3(0.f, 0.f, 0.f));
	setRotation(glm::quat(glm::vec3(0.f, 0.f, 0.f)));
	setScale(glm::vec3(1.f, 1.f, 1.f));
}

SceneNode::~SceneNode() {

	if(mRigidBody != NULL) delete mRigidBody->getMotionState();
	if(mRigidBody != NULL) delete mRigidBody;
	if(mCollisionShape != NULL) delete mCollisionShape;

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
		btTransform transform;
		if(mRigidBody != NULL) {
			mRigidBody->getMotionState()->getWorldTransform(transform);
			mPosition = glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
			mRotation = glm::quat(transform.getRotation().w(), transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z());
		}
		glm::mat4 scale = glm::scale(glm::mat4(1.f), mScale);
		glm::mat4 translation = glm::translate(glm::mat4(1.f), mPosition);
		glm::mat4 rotation = glm::toMat4(mRotation);
		return scale * translation * rotation;
}

void SceneNode::setPosition(glm::vec3 position) { 
	mPosition = position;
	if(mRigidBody == NULL)
		return;
	btTransform transform = mRigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(mPosition.x, mPosition.y, mPosition.z));
	mRigidBody->setCenterOfMassTransform(transform);
}

void SceneNode::setRotation(glm::quat rotation) { 
	mRotation = rotation;
	if(mRigidBody == NULL)
		return;
	btTransform transform = mRigidBody->getCenterOfMassTransform();
	transform.setRotation(btQuaternion(mRotation.x, mRotation.y, mRotation.z, mRotation.w));
	mRigidBody->setCenterOfMassTransform(transform);
}

void SceneNode::setRotation(glm::vec3 rotation) { 
	mRotation = glm::quat(rotation);
	if(mRigidBody == NULL)
		return;
	btTransform transform = mRigidBody->getCenterOfMassTransform();
	transform.setRotation(btQuaternion(mRotation.x, mRotation.y, mRotation.z, mRotation.w));
	mRigidBody->setCenterOfMassTransform(transform);
}