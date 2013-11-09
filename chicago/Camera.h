#ifndef _CAMERA_H_
#define _CAMERA_H_


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtx\quaternion.hpp"

class Camera {

private:
	float mFOV;
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;
	glm::mat4 mView;
	void updateView();

public:
	Camera();
	void setFOV(float fov) { mFOV = fov; }
	float getFOV() { return mFOV; }
	void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
	void lookAt(glm::vec3 target);
	void setPosition(glm::vec3 position);
	void setRotation(glm::quat rotation);
	void setRotation(glm::vec3 euler);
	glm::mat4 getView();
};

#endif