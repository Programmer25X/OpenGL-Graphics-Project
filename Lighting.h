#pragma once
#ifndef LIGHTING_CLASS_H
#define LIGHTING_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Lighting
{
public:

	glm::vec3 lightPosition = glm::vec3(120.0f, 120.0f, 200.0f);
	
	std::vector<GLfloat> verticies =
	{
		// Coordinates  

	-50.0f, -50.0f, -50.0f,
	 50.0f, -50.0f, -50.0f,
	 50.0f,  50.0f, -50.0f,
	 50.0f,  50.0f, -50.0f,
	-50.0f,  50.0f, -50.0f,
	-50.0f, -50.0f, -50.0f,

	-50.0f, -50.0f,  50.0f,
	 50.0f, -50.0f,  50.0f,
	 50.0f,  50.0f,  50.0f,
	 50.0f,  50.0f,  50.0f,
	-50.0f,  50.0f,  50.0f,
	-50.0f, -50.0f,  50.0f,

	-50.0f,  50.0f,  50.0f,
	-50.0f,  50.0f, -50.0f,
	-50.0f, -50.0f, -50.0f,
	-50.0f, -50.0f, -50.0f,
	-50.0f, -50.0f,  50.0f,
	-50.0f,  50.0f,  50.0f,

	 50.0f,  50.0f,  50.0f,
	 50.0f,  50.0f, -50.0f,
	 50.0f, -50.0f, -50.0f,
	 50.0f, -50.0f, -50.0f,
	 50.0f, -50.0f,  50.0f,
	 50.0f,  50.0f,  50.0f,

	-50.0f, -50.0f, -50.0f,
	 50.0f, -50.0f, -50.0f,
	 50.0f, -50.0f,  50.0f,
	 50.0f, -50.0f,  50.0f,
	-50.0f, -50.0f,  50.0f,
	-50.0f, -50.0f, -50.0f,

	-50.0f,  50.0f, -50.0f,
	 50.0f,  50.0f, -50.0f,
	 50.0f,  50.0f,  50.0f,
	 50.0f,  50.0f,  50.0f,
	-50.0f,  50.0f,  50.0f,
	-50.0f,  50.0f, -50.0f,

	};

	std::vector<GLuint> indices =
	{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7

	};

private:

};

class PointLight : public Lighting
{

};

class DirectionalLight : public Lighting
{

};

class SpotLight : public Lighting
{

};

#endif