#include "Lighting.h"


Lighting::Lighting(const glm::vec4& pLightColor)
{
	lightColor = pLightColor; 
}

const glm::vec4 Lighting::getLightColor() const { return lightColor; }

const glm::vec3 Lighting::getLightPosition() const { return lightPosition; }

const glm::vec3 Lighting::getLightDirection() const { return lightDirection; }

const GLfloat Lighting::getLightIntensity() const { return lightIntensity; }

const std::vector<GLuint> Lighting::getIndices() const { return indices; }

const std::vector<GLfloat> Lighting::getVerticies() const { return verticies; }
