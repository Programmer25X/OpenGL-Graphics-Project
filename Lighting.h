#pragma once
#ifndef LIGHTING_CLASS_H
#define LIGHTING_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Lighting
{

public:
	Lighting(const glm::vec3& pLightColor = glm::vec3(1.0f, 1.0f, 1.0f));

protected:

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(120.0f, 120.0f, 200.0f);
	glm::vec3 lightDirection = glm::vec3(0, 0, 0);
	GLfloat lightIntensity = 1.0f;
	
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

public:
	const glm::vec3 getLightColor() const;
	const glm::vec3 getLightPosition() const;
	const glm::vec3 getLightDirection() const;
	const GLfloat getLightIntensity() const;
	const std::vector<GLuint> getIndices() const;
	const std::vector<GLfloat> getVerticies() const;

	void setLightPosition(const GLfloat x, const GLfloat y, const GLfloat z);
	void setLightColor(const GLfloat r, const GLfloat g, const GLfloat b);

};

class AmbientLight : public Lighting
{
public:
	AmbientLight(const glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.1f);

private:
	GLfloat ambientStrength = 0.0f;
	glm::vec3 ambient = glm::vec3(0, 0, 0); 

public:
	const glm::vec3 getAmbient() const; 
	const GLfloat getAmbientStrength() const; 

	void setAmbientColor(const GLfloat r, const GLfloat g, const GLfloat b);
	void setAmbientStrength(const GLfloat pAmbientStrength);

};




class DiffuseLight : public Lighting
{
public:
	DiffuseLight(const glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f));
};




class SpecularLight : public Lighting
{
public:
	SpecularLight(GLint pShininessValue = 32);

private:
	GLint shininessValue = 0;

public:
	const GLint getShininessValue() const;
	void setShininessValue(const GLint pShininessValue);
};

#endif