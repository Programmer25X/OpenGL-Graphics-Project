#include "Camera.h"
#include "Renderer.h"

#include <cmath>
#include <algorithm>

Camera::Camera()
{
	// View Space 
	
	glm::vec3 frontDirection(0.0f, 0.0f, 0.0f);
	frontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontDirection.y = sin(glm::radians(pitch));
	frontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(frontDirection);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); // Gets the view matrix for the MVP matrix
}

glm::mat4 Camera::getProjectionMatrix(const GLfloat bufferWidth, const GLfloat bufferHeight, const GLfloat nearPlane, const GLfloat farPlane) const
{
	return glm::perspective(glm::radians(fieldOfView), static_cast<GLfloat>(bufferWidth / bufferHeight), nearPlane, farPlane); 
}


void Camera::processCameraInputs(const GLfloat cameraSpeed, GLFWwindow* window)
{
	enum class MoveDirection {NONE ,UP, DOWN, LEFT, RIGHT};
	MoveDirection userInput = MoveDirection::NONE;

	GLfloat deltaTime = 0.0f;
	GLfloat previousFrame = 0.0f;
	GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
	
	deltaTime = currentFrame - previousFrame;
	previousFrame = currentFrame;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		userInput = MoveDirection::UP;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		userInput = MoveDirection::LEFT;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		userInput = MoveDirection::DOWN;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		userInput = MoveDirection::RIGHT;
	}
	else
	{
		userInput = MoveDirection::NONE;
		return;
	}

	switch (userInput)
	{
	case MoveDirection::UP:
		cameraPosition += cameraSpeed * deltaTime * cameraFront; // Move camera up
		break;
	case MoveDirection::LEFT:
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime; // Move camera left
		break;
	case MoveDirection::DOWN:
		cameraPosition -= cameraSpeed * deltaTime * cameraFront; // Move camera down
		break;
	case MoveDirection::RIGHT:
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime; // Move camera right
		break;
	case MoveDirection::NONE:
		break;
	default:
		break;
	}
}

void Camera::processMouseMovements(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{ 
	xOffset *= MOUSE_SENSITIVITY;
	yOffset *= MOUSE_SENSITIVITY;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		pitch = std::ranges::clamp(pitch, -89.0f, 89.0f);
	}

	glm::vec3 frontDirection(0.0f, 0.0f, 0.0f);
	frontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontDirection.y = sin(glm::radians(pitch));
	frontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(frontDirection); // Update the camera's local forward direction
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp)); // Update the camera's local right direction
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront)); // Update the camera's local up direction
}

void Camera::processMouseScroll(GLfloat yOffset)
{
	fieldOfView -= yOffset;
	fieldOfView = std::ranges::clamp(fieldOfView, 1.0f, 45.0f);
}







