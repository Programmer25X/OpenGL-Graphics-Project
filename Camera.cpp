#include "Camera.h"
#include "Renderer.h"


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
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
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
		cameraPosition += cameraSpeed * deltaTime * cameraFront;
		break;
	case MoveDirection::LEFT:
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		break;
	case MoveDirection::DOWN:
		cameraPosition -= cameraSpeed * deltaTime * cameraFront;
		break;
	case MoveDirection::RIGHT:
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
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
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		else if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	glm::vec3 frontDirection(0.0f, 0.0f, 0.0f);
	frontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontDirection.y = sin(glm::radians(pitch));
	frontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(frontDirection);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}







