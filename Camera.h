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
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;

	const GLfloat MOUSE_SENSITIVITY = 0.1f;

	glm::mat4 viewMatrix; 

public:
	glm::mat4 getViewMatrix() const; 

	void processCameraInputs(const GLfloat cameraSpeed, GLFWwindow* window);
	void processMouseMovements(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = GL_TRUE);
};



#endif