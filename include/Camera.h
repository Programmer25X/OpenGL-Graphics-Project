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

	const GLfloat MOUSE_SENSITIVITY = 0.1f;

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;

	GLfloat fieldOfView = 45.0f;
	GLfloat nearPlane = 0.01f;
	GLfloat farPlane = 5000.0f;
	GLfloat cameraSpeed = 600.0f; 

	glm::mat4 viewMatrix = glm::mat4(0.0f);
	glm::mat4 projectionMatrix = glm::mat4(0.0f); 

public:
	const glm::mat4 getViewMatrix() const; 
	const glm::mat4 getProjectionMatrix(const GLfloat bufferWidth, const GLfloat bufferHeight, const GLfloat nearPlane = 0.1f, const GLfloat farPlane = 1000.0f) const;
	const glm::vec3 getPosition() const;
	const glm::vec3 getFront() const;
	const GLfloat getFOV() const;
	const GLfloat getNearPlane() const;
	const GLfloat getFarPlane() const;
	const GLfloat getCameraSpeed() const; 

	void setFOV(const GLfloat pFOV);
	void setNearPlane(const GLfloat pNearPlane);
	void setFarPlane(const GLfloat pFarPlane);
	void setCameraSpeed(const GLfloat pCameraSpeed); 

	void processCameraInputs(GLFWwindow* window);
	void processMouseMovements(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = GL_TRUE);
	void processMouseScroll(GLfloat yOffset); 
};


#endif