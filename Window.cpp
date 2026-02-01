#include "Window.h"
#include "Renderer.h"

#include <iostream>


EngineWindow::EngineWindow(const std::string& windowTitle)
{	
	if (!glfwInit())
	{
		return;
	}

	windowPtr = glfwCreateWindow(screenWidth, screenHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
}

GLFWwindow* EngineWindow::getWindow() const { return windowPtr; }

const GLint EngineWindow::getWidth() const { return screenWidth; }

const GLint EngineWindow::getHeight() const { return screenHeight; }

const GLfloat EngineWindow::getAspectRatio() const { return aspectRatio; }


const void EngineWindow::setAspectRatio(const GLint bufferWidth, const GLint bufferHeight)
{
	aspectRatio = static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight);
}


void EngineWindow::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

