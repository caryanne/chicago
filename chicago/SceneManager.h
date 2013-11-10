#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_


#include <vector>
#include "SceneNode.h"
#include "Camera.h"


using namespace std;

class SceneManager {

private:
	SceneNode mRootNode;
	Camera mCamera;
	float mRatio;
	void drawNode(SceneNode* sceneNode);
	void reloadNode(SceneNode* sceneNode);
	glm::mat4 mProjection;
	glm::mat4 mView;
	glm::mat4 mViewProjection;
	void updateMatrices();
	glm::vec4 mLightPos;

public:
	SceneManager();
	SceneNode *getRootNode() { return &mRootNode; }	
	float getScreenRatio() { return mRatio; }
	void setScreenRatio(float ratio) { mRatio = ratio; updateMatrices(); }
	void setLightPos(glm::vec4 position) { mLightPos = position; }
	void drawScene();
	void reloadScene();

	void setCamera(Camera camera) { mCamera = camera; updateMatrices(); }
	Camera* getCamera() { return &mCamera; }

	//skybox stuff
};

#endif