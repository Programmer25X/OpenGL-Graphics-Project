#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

class Camera
{
public:
	Camera(GLFWwindow* window);

private:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;

	const GLfloat mouseSensitity = 0.1f;
	GLfloat lastXPosition = 400.0f;
	GLfloat lastYPosition = 300.0f;


public:
	const glm::vec3 getCameraPosition() const;
	const glm::vec3 getCameraFront() const;
	const glm::vec3 getCameraUp() const;

	GLfloat getLastXPosition();
	GLfloat getLastYPosition(); 

	const GLfloat getMouseSensitivity() const; 

	GLfloat getYaw();
	GLfloat getPitch();

	void processCameraInputs(const GLfloat cameraSpeed, GLFWwindow* window);
};



#endif