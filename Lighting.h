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
protected:

	glm::vec3 lightPosition = glm::vec3(120.0f, 120.0f, 200.0f);
	
	std::vector<GLfloat> verticies =
	{

	-100.0f, -100.0f, -100.0f,  0.0f, 0.0f,
	 100.0f, -100.0f, -100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 0.0f,

	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 1.0f,
	-100.0f,  100.0f,  100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,

	-100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,  1.0f, 0.0f,

	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,

	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f, -100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f, -100.0f, -100.0f,  0.0f, 1.0f,

	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,  1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,  1.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,  0.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,  0.0f, 1.0f,

	};

	std::vector<GLuint> indices =
	{
		0, 1, 2,
		2, 3, 0
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