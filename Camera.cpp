#include "Camera.h"
#include "Renderer.h"


Camera::Camera(GLFWwindow* window)
{
	// View Space 

	cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f); // Sets inital camera position 
	direction = glm::normalize(cameraPosition - cameraTarget); // Sets the initial camera direction
	
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, direction)); // Sets the camera's right direction

	glm::vec3 cameraUp = glm::cross(direction, cameraRight); // Sets the camera's up direction

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = cos(glm::radians(pitch)); 
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	LOG_ERRORS(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}



const glm::vec3 Camera::getCameraPosition() const { return cameraPosition; }

const glm::vec3 Camera::getCameraFront() const { return cameraFront; }

const glm::vec3 Camera::getCameraUp() const { return cameraUp; }

GLfloat Camera::getLastXPosition() { return lastXPosition; }

GLfloat Camera::getLastYPosition() { return lastYPosition; }

const GLfloat Camera::getMouseSensitivity() const { return mouseSensitity; }

GLfloat Camera::getYaw() { return yaw; }

GLfloat Camera::getPitch() { return pitch; }



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



