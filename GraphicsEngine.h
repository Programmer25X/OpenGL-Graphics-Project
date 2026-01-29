#pragma once

#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"


class GraphicsEngine
{
public:
	GraphicsEngine(EngineWindow* pWindow);
	~GraphicsEngine(); 

private:
	EngineWindow* engineWindow;

public:
	void run();

};

#endif
