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


// ======================================== Lighting ======================================================= //

class Lighting
{

public:
	Lighting(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.2f, const GLfloat pDiffuseStrength = 0.5f, const GLint pShininessValue = 32);

protected:

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	
	GLfloat ambientStrength = 0.1f;
	glm::vec3 ambientColor = glm::vec3(0, 0, 0);

	GLfloat diffuseStrength = 0.1f;
	glm::vec3 diffuseColor = glm::vec3(0, 0, 0);

	glm::vec3 lightPosition = glm::vec3(120.0f, 120.0f, 200.0f);

	GLint shininessValue = 0;

	
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

	const std::vector<GLuint> getIndices() const;
	const std::vector<GLfloat> getVerticies() const;

	void setLightColor(const glm::vec3& pLightColor);

	// Ambient Lighting 
	const glm::vec3 getAmbientColor() const;
	const GLfloat getAmbientStrength() const;
	void setAmbientColor(); 
	void setAmbientStrength(const GLfloat pAmbientStrength);

	// Diffuse Lighting
	const glm::vec3 getLightPosition() const;
	const glm::vec3 getDiffuseColor() const;
	const GLfloat getDiffuseStrength() const; 
	void setLightPosition(const glm::vec3& pLightPosition);
	void setDiffuseColor();
	void setDiffuseStrength(const GLfloat pDiffuseStrength);
	
	// Specular Lighting 
	const GLint getShininessValue() const;
	void setShininessValue(const GLint pShininessValue);

};

// ======================================== Directional Lighting ======================================================= //

class DirectionalLight : public Lighting
{

public:
	DirectionalLight(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.2f, const GLfloat pDiffuseStrength = 0.5f, const GLint pShininessValue = 32);

private:
	glm::vec3 lightDirection = glm::vec3(0, 0, 0);

public:
	const glm::vec3 getLightDirection() const;

};


// ======================================== Point Light ============================================================= //

class PointLight : public Lighting
{
public:
	PointLight(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.2f, const GLfloat pDiffuseStrength = 0.5f, const GLint pShininessValue = 32);

private:
	GLfloat constant = 1;
	GLfloat linear = 0.09f;
	GLfloat quadratic = 0.032f;
};


// ======================================== Spot Light ================================================================== //

class SpotLight : public Lighting
{

};

#endif