#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <gl\glew.h>
#include "glfw\glfw3.h"
#include <vector>
#include "SceneNode.h"
#include "Camera.h"


using namespace std;

class SceneManager {

private:
	SceneNode mRootNode;
	Camera *mCamera;

public:
	SceneManager() {}
	SceneNode *getRootNode() { return &mRootNode; }	
	void drawScene();

	void setCamera(Camera* camera) { mCamera = camera; }
	const Camera* getCamera() { return mCamera; }

	//skybox stuff
};

#endif