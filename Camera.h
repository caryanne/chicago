#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "MoveableObject.h"

class Camera: public MoveableObject {

private:
	float mFOV;

public:
	Camera();
	void setFOV(float fov) { mFOV = fov; }

};

#endif