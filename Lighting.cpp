#include "Lighting.h"


Lighting::Lighting(const glm::vec3& pLightColor, const GLfloat pAmbientStrength, const GLfloat pDiffuseStrength,  const GLint pShininessValue)
{
	lightColor = pLightColor;
	ambientStrength = pAmbientStrength;
	shininessValue = pShininessValue;
	lightDirection = -lightPosition; 
}

const glm::vec3 Lighting::getLightColor() const { return lightColor; }

const glm::vec3 Lighting::getLightDirection() const { return lightDirection; }

const std::vector<GLuint> Lighting::getIndices() const { return indices; }

const std::vector<GLfloat> Lighting::getVerticies() const { return verticies; }


void Lighting::setLightColor(const glm::vec3& pLightColor)
{
	lightColor = pLightColor; 
}


// Ambient Lighting 

const glm::vec3 Lighting::getAmbientColor() const { return ambientColor; }

const GLfloat Lighting::getAmbientStrength() const { return ambientStrength; }

void Lighting::setAmbientColor()
{
	ambientColor = lightColor * ambientStrength;
}

void Lighting::setAmbientStrength(const GLfloat pAmbientStrength)
{
	ambientStrength = pAmbientStrength;
}




// Diffuse Lighting 


void Lighting::setLightPosition(const glm::vec3& pLightPosition)
{
	lightPosition = pLightPosition;
}

void Lighting::setDiffuseColor()
{
	diffuseColor = lightColor * diffuseStrength;
}

void Lighting::setDiffuseStrength(const GLfloat pDiffuseStrength)
{
	diffuseStrength = pDiffuseStrength; 
}

const GLfloat Lighting::getDiffuseStrength() const { return diffuseStrength; }

const glm::vec3 Lighting::getDiffuseColor() const { return diffuseColor; }

const glm::vec3 Lighting::getLightPosition() const { return lightPosition; }



// Specular Lighting 

const GLint Lighting::getShininessValue() const { return shininessValue; }

void Lighting::setShininessValue(const GLint pShininessValue)
{
	shininessValue = pShininessValue;
}

