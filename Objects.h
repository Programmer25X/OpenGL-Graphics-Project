#pragma once
#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Object
{
public:

};

class LogoCube : public Object
{
private:
	std::vector<GLfloat> verticies =
	{
		// Coordinates			  Normals	           Texture Coordinates 

	-100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
	 100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	-100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
		
	-100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

	-100.0f,  100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 100.0f,  100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

	-100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	-100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	-100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

	-100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f

	};

	std::vector<GLuint> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

public:

	const glm::vec3 getCubePosition() const;
	const std::vector<GLuint> getIndices() const; 
	const std::vector<GLfloat> getVerticies() const;
};

#endif

