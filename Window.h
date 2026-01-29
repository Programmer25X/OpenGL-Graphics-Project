#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class EngineWindow
{

public: 
	EngineWindow(const std::string& windowTitle);

private:
	GLFWwindow* windowPtr = nullptr; 
	GLint screenWidth = 1280;
	GLint screenHeight = 720;
	GLfloat aspectRatio = 0.0f; 


public:
	GLFWwindow* getWindow() const; 
	const GLint getWidth() const;
	const GLint getHeight() const; 
	const GLint getAspectRatio() const;
	const void setAspectRatio(const GLint bufferWidth, const GLint bufferHeight);


	void processInput(GLFWwindow* window); 
};

#endif
