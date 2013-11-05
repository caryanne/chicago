#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "MoveableObject.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtx\quaternion.hpp"

class Camera: public MoveableObject {

private:
	float mFOV;

public:
	Camera();
	void setFOV(float fov) { mFOV = fov; }
	float getFOV() { return mFOV; }
	void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
	void lookAt(glm::vec3 target);
	void setPosition(glm::vec3 position);
	void setRotation(glm::quat rotation);
	void setRotation(glm::vec3 euler);


};

#endif