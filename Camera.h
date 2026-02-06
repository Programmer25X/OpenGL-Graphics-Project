#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Camera
{
public:
	Camera();

private:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 359.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	const GLfloat MOUSE_SENSITIVITY = 0.1f;

	GLfloat fieldOfView = 45.0f;

	glm::mat4 viewMatrix = glm::mat4(0.0f);
	glm::mat4 projectionMatrix = glm::mat4(0.0f); 

public:
	glm::mat4 getViewMatrix() const; 
	glm::mat4 getProjectionMatrix(const GLfloat bufferWidth, const GLfloat bufferHeight, const GLfloat nearPlane = 0.1f, const GLfloat farPlane = 1000.0f) const;

	void processCameraInputs(const GLfloat cameraSpeed, GLFWwindow* window);
	void processMouseMovements(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = GL_TRUE);
	void processMouseScroll(GLfloat yOffset); 
};



#endif