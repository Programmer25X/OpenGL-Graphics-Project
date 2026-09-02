#pragma once

#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Shader.h"
#include "Window.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include<glad/glad.h>

// MACROS 
#define ASSERT(x) if (!(x)) __debugbreak(); 

#ifdef _DEBUG
#define LOG_ERRORS(x) clearErrors();x;ASSERT(checkAndDisplayErrors(#x, __FILE__, __LINE__))
#else
#define LOG_ERRORS(x) x
#endif

void clearErrors();
bool checkAndDisplayErrors(const char* functionName, const char* fileName, int line);

class Renderer
{

public:
	void draw(const VertexArrayObject& VAO, const ElementBufferObject& EBO, const Shader& shader); 
	void clear(); 
	static void setupBlendFunctions();

};

#endif