#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <vector>
#include "SceneNode.h"

using namespace std;

class SceneManager {

private:
	SceneNode mRootNode;


public:
	SceneManager();
	
	SceneNode *getRootNode() { return &mRootNode; }	
	void drawScene();

	void setCamera();
	void createCamera();
	void getCamera();

	//skybox stuff
};

#endif