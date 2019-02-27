#include "Camera.h"

Camera::Camera(){
    this->position = glm::vec3(0.0f, 0.0f, 40.0f); /*!< Position of the camera */
	this->front = glm::vec3(0.0f, 0.0f, -1.0f); /*!< Front of the camera */
	this->up = glm::vec3(0.0f, 1.0f, 0.0f); /*!< Up of the camera */
	this->fov = 45.0f; /*!< FOV of the camera */
	this->speed = 1.0f;
}

Camera::~Camera(){}

glm::vec3 Camera::right() const {
	return  glm::normalize(glm::cross(this->front, this->up));
}

void Camera::pitch(float degree){
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree), this->right());
	this->front = glm::vec3(glm::vec4(this->front, 0) * rotation);
	this->up = glm::vec3(glm::vec4(this->up, 0) * rotation);
}

void Camera::yaw(float degree) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree), this->up);
	this->front = glm::vec3(glm::vec4(this->front, 0) * rotation);
}


void Camera::moveForward(){
    this->position += this->speed * this->front;
}

void Camera::moveBackward(){
    this->position -= this->speed * this->front;
}

void Camera::moveRight(){
    this->position += this->speed * this->right();
}

void Camera::moveLeft(){
    this->position -= this->speed * this->right();
}

glm::vec3 Camera::getPosition() const {
    return this->position;
}

glm::vec3 Camera::getFront() const {
    return this->front;
}

glm::vec3 Camera::getUp() const {
    return this->up;
}

float Camera::getFov() const {
    return this->fov;
}

//! Reset the camera to its initial configurations.
void Camera::reset() {
	this->position = glm::vec3(0.0f, 0.0f, 55.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->fov = 45.0f;
	this->speed = 1.0f;
}