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
	Lighting(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.2f, const GLfloat pDiffuseStrength = 0.5f, const GLfloat pSpecularStrength = 1.0f, const GLint pShininessValue = 32);

protected:

	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	
	GLfloat ambientStrength = 0.1f;
	glm::vec3 ambientColour = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat diffuseStrength = 0.1f;
	glm::vec3 diffuseColour = glm::vec3(0.0f, 0.0f, 0.0f);
	
	GLfloat specularStrength = 0.1f;
	glm::vec3 specularColour = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 lightPosition = glm::vec3(120.0f, 120.0f, 200.0f);
	glm::vec3 lightDirection = glm::vec3(0, 0, 0);

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

	void setLightColour(const glm::vec3& pLightColor);

	const glm::vec3 getLightPosition() const;
	const glm::vec3 getLightDirection() const;

	// Ambient Lighting 
	const glm::vec3 getAmbientColour() const;
	const GLfloat getAmbientIntensity() const;

	void setAmbientColour(); 
	void setAmbientIntensity(const GLfloat pAmbientStrength);

	// Diffuse Lighting
	const glm::vec3 getDiffuseColour() const;
	const GLfloat getDiffuseIntensity() const; 

	void setLightPosition(const glm::vec3& pLightPosition);
	void setDiffuseColour();
	void setDiffuseIntensity(const GLfloat pDiffuseStrength);
	
	// Specular Lighting 
	const GLfloat getSpecularIntensity() const;
	const GLint getShininessValue() const;
	const glm::vec3 getSpecularColour() const;

	void setSpecularIntensity(const GLfloat pSpecularStrength);
	void setSpecularColour();
	void setShininessValue(const GLint pShininessValue);

};

// ======================================== Directional Lighting ======================================================= //

class DirectionalLight : public Lighting
{

public:
	DirectionalLight(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pAmbientStrength = 0.0125f, const GLfloat pDiffuseStrength = 0.0f, const GLfloat pSpecularStrength = 1.0f, const GLint pShininessValue = 32);
};


// ======================================== Point Light ============================================================= //

class PointLight : public Lighting
{
public:
	PointLight(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pConstant = 1.0f, const GLfloat pLinear = 0.0014f, const GLfloat pQuadratic = 0.000007f, const GLfloat pAmbientStrength = 0.2f, const GLfloat pDiffuseStrength = 0.5f, const GLfloat pSpecularStrength = 1.0f, const GLint pShininessValue = 32);

private:
	GLfloat constant = 0.0f;
	GLfloat linear = 0.0f;
	GLfloat quadratic = 0.0f;

public:
	const GLfloat getConstant() const; 
	const GLfloat getLinear() const;
	const GLfloat getQuadratic() const; 

	void setConstant(const GLfloat pConstant);
	void setLinear(const GLfloat PLinear);
	void setQuadratic(const GLfloat pQuadratic);
};


// ======================================== Spot Light ================================================================== //

class SpotLight : public Lighting
{
public:
	SpotLight(const glm::vec3& pLightColor = glm::vec3(0.0f, 1.0f, 1.0f), const GLfloat pInnerCutOff = glm::cos(glm::radians(12.5f)), const GLfloat pOuterCutOff = glm::cos(glm::radians(15.0f)), const GLfloat pConstant = 1.0f, const GLfloat pLinear = 0.0014f, const GLfloat pQuadratic = 0.000007f, const GLfloat pAmbientStrength = 2.0f, const GLfloat pDiffuseStrength = 2.0f, const GLfloat pSpecularStrength = 1.0f, const GLint pShininessValue = 32);

private:
	GLfloat innerCutOff = 0.0f;
	GLfloat outerCutOff = 0.0f; 

	GLfloat constant = 0.0f;
	GLfloat linear = 0.0f;
	GLfloat quadratic = 0.0f;

public:
	const GLfloat getInnerCutOff() const;
	const GLfloat getOuterCutOff() const; 

	const GLfloat getConstant() const;
	const GLfloat getLinear() const;
	const GLfloat getQuadratic() const;

	void setInnerCutOff(const GLfloat pInnerCutOff);
	void setOuterCutOff(const GLfloat pOuterCutOff); 

	void setConstant(const GLfloat pConstant);
	void setLinear(const GLfloat PLinear);
	void setQuadratic(const GLfloat pQuadratic);
};

#endif