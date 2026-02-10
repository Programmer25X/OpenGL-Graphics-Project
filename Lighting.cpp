#include "Lighting.h"

// Base Light Class 

Lighting::Lighting(const glm::vec3& pLightColor)
{
	lightColor = pLightColor; 
}

const glm::vec3 Lighting::getLightColor() const { return lightColor; }

const std::vector<GLuint> Lighting::getIndices() const { return indices; }

const std::vector<GLfloat> Lighting::getVerticies() const { return verticies; }

void Lighting::setLightColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
	lightColor.r = r;
	lightColor.g = g;
	lightColor.b = b;
}



// Ambient Light Class 

AmbientLight::AmbientLight(const glm::vec3 ambientColor, const GLfloat pAmbientStrength)
{
	ambientStrength = pAmbientStrength;
	ambient = ambientStrength * lightColor; 
}

const glm::vec3 AmbientLight::getAmbient() const { return ambient; }

const GLfloat AmbientLight::getAmbientStrength() const { return ambientStrength; }

void AmbientLight::setAmbientColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
	lightColor.r = r;
	lightColor.g = g;
	lightColor.b = b;
	ambient = ambientStrength * lightColor;
}

void AmbientLight::setAmbientStrength(const GLfloat pAmbientStrength)
{
	ambientStrength = pAmbientStrength;
	ambient = ambientStrength * lightColor;
}


// Diffuse Light Class 


DiffuseLight::DiffuseLight(const glm::vec3 diffuseColor)
{
	lightColor = diffuseColor;
}

const glm::vec3 DiffuseLight::getLightPosition() const { return lightPosition; }

const glm::vec3 DiffuseLight::getLightDirection() const { return lightDirection; }

void DiffuseLight::setLightPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
	
}


// Specular Light Class 

SpecularLight::SpecularLight(GLint pShininessValue)
{
	shininessValue = pShininessValue;
}

const GLint SpecularLight::getShininessValue() const { return shininessValue; }

void SpecularLight::setShininessValue(const GLint pShininessValue)
{
	shininessValue = pShininessValue;
}

