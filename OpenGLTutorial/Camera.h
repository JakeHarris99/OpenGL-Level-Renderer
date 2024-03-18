#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

class Camera {
public:
	
	Camera() {
		float fov = 45.0f;
		float aspectRatio = 640.0f/480.0f;
		float near = 0.1f;
		float far = 10000.0f;
		this->position = glm::vec3(0.0f, 0.0f, -5.0f);
		this->lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
		projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
		viewMatrix = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void updateViewMatrix() {
		viewMatrix = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 getViewMatrix() {
		return viewMatrix;
	}

	glm::mat4 getprojectionMatrix() {
		return projectionMatrix;
	}

	glm::vec3 getPosition() {
		return position;
	}

	void setPosition(float x, float y, float z) {
		this->position = glm::vec3(x, y, z);
	}

	glm::vec3 getLookAt() {
		return lookAt;
	}

	void setLookAt(float x, float y, float z) {
		this->lookAt = glm::vec3(x, y, z);
	}

protected:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 position;
	glm::vec3 lookAt;
};