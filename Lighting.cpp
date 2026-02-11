#include "Lighting.h"


Lighting::Lighting(const glm::vec3& pLightColor, const GLfloat pAmbientStrength, const GLint pShininessValue)
{
	lightColor = pLightColor;
	ambientStrength = pAmbientStrength;
	shininessValue = pShininessValue;
}

const glm::vec3 Lighting::getLightColor() const { return lightColor; }

const glm::vec3 Lighting::getLightPosition() const { return lightPosition; }

const glm::vec3 Lighting::getLightDirection() const { return lightDirection; }

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


const glm::vec3 Lighting::getAmbient() const { return ambient; }

const GLfloat Lighting::getAmbientStrength() const { return ambientStrength; }

void Lighting::setAmbientColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
	lightColor.r = r;
	lightColor.g = g;
	lightColor.b = b;
	ambient = ambientStrength * lightColor;
}

void Lighting::setAmbientStrength(const GLfloat pAmbientStrength)
{
	ambientStrength = pAmbientStrength;
	ambient = ambientStrength * lightColor;
}

const GLint Lighting::getShininessValue() const { return shininessValue; }

void Lighting::setShininessValue(const GLint pShininessValue)
{
	shininessValue = pShininessValue;
}

