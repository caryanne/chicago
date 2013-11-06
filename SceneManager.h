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
	void drawNode(SceneNode* sceneNode);
	void reloadNode(SceneNode* sceneNode);

public:
	SceneManager() { mRootNode = SceneNode(); }
	SceneNode *getRootNode() { return &mRootNode; }	
	void drawScene();
	void reloadScene();

	void setCamera(Camera camera) { mCamera = camera; }
	Camera* getCamera() { return &mCamera; }

	//skybox stuff
};

#endif