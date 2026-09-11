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
	Object();

protected:
	std::vector<GLfloat> verticies = {};
	std::vector<GLuint> indices = {};
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	GLuint shininess = 1;


public:
	const std::vector<GLuint> getIndices() const;
	const std::vector<GLfloat> getVerticies() const;
	const glm::vec3 getPosition() const;
	const GLuint getShininess() const;

	void setShininess(GLuint pShininess = 1); 
};

class Box : public Object
{

public:
	Box(); 
};

#endif

