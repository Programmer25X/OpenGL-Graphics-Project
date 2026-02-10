#include "Lighting.h"

// Base Light Class 

Lighting::Lighting(const glm::vec3& pLightColor)
{
	lightColor = pLightColor; 
}

const glm::vec3 Lighting::getLightColor() const { return lightColor; }

const glm::vec3 Lighting::getLightPosition() const { return lightPosition; }

const glm::vec3 Lighting::getLightDirection() const { return lightDirection; }

const GLfloat Lighting::getLightIntensity() const { return lightIntensity; }

const std::vector<GLuint> Lighting::getIndices() const { return indices; }

const std::vector<GLfloat> Lighting::getVerticies() const { return verticies; }

void Lighting::setLightPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
	lightPosition.x = x;
	lightDirection.y = y;
	lightPosition.z = z; 
}

void Lighting::setLightColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
	lightColor.r = r;
	lightColor.g = g;
	lightColor.b = b;
}



// Ambient Light Class 

AmbientLight::AmbientLight(const GLfloat pAmbientStrength)
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
