#include "Camera.h"

Camera::Camera() {
	lookAt(glm::vec3(10.f, 10.f, 10.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
}

void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
	mView = glm::lookAt(position, target, up);
}

void Camera::lookAt(glm::vec3 target) {
	mView = glm::lookAt(mPosition, target, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::setPosition(glm::vec3 position) {
	mPosition = position;
	updateView();
}

void Camera::setRotation(glm::quat rotation) {
	mRotation = rotation;
	updateView();
}

void Camera::setRotation(glm::vec3 euler) {
	mRotation = glm::quat(euler);
	updateView();
}

void Camera::updateView() {
	mView = glm::translate(glm::mat4(1.f), mPosition) * glm::toMat4(mRotation);
}

glm::mat4 Camera::getView() {
	return mView;
}