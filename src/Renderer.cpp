#include "Renderer.h"

#include <iostream>
#include <format>


void clearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool checkAndDisplayErrors(const char* functionName, const char* fileName, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << std::format("OpenGL Error {}\nFunction: {}\nFile: {}\nLine: {}", error, functionName, fileName, line) << std::endl;
		return false;
	}

	return true;
}

/// <summary>
/// Draws/renders an object based on parameters
/// </summary>
/// <param name="VAO"></param>
/// <param name="EBO"></param>
/// <param name="shader"></param>
void Renderer::draw(const VertexArrayObject& VAO, const ElementBufferObject& EBO, const Shader& shader)
{
	shader.useShader(); 
	LOG_ERRORS(VAO.bind());
	LOG_ERRORS(EBO.bind());

	glDrawArrays(GL_TRIANGLES, 0, 36);
	// LOG_ERRORS(glDrawElements(GL_TRIANGLES, EBO.getElementCount(), GL_UNSIGNED_INT, NULL))
}

void Renderer::clear()
{
	LOG_ERRORS(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); 
}

void Renderer::setupBlendFunctions()
{
	LOG_ERRORS(glEnable(GL_BLEND)) // Enables blending
	LOG_ERRORS(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Defines how OpenGL blends alpha pixels
	LOG_ERRORS(glEnable(GL_DEPTH_TEST)); 
}
