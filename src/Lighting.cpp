#include "Lighting.h"


Lighting::Lighting(const glm::vec3& pLightColor, const GLfloat pAmbientStrength, const GLfloat pDiffuseStrength, const GLfloat pSpecularStrength, const GLint pShininessValue)
{
	lightColour = pLightColor;
	ambientStrength = pAmbientStrength;
	diffuseStrength = pDiffuseStrength;
	specularStrength = pSpecularStrength; 
	shininessValue = pShininessValue;
}

const glm::vec3 Lighting::getLightColour() const { return lightColour; }

const std::vector<GLuint> Lighting::getIndices() const { return indices; }

const std::vector<GLfloat> Lighting::getVerticies() const { return verticies; }

const glm::vec3 Lighting::getLightDirection() const { return lightDirection; }

void Lighting::setLightColour(const glm::vec3& pLightColor) { lightColour = pLightColor; }

void Lighting::setLightPosition(const glm::vec3& pLightPosition) { lightPosition = pLightPosition; }


// Ambient Lighting 

const glm::vec3 Lighting::getAmbientColour() const { return ambientColour; }

const GLfloat Lighting::getAmbientIntensity() const { return ambientStrength; }

void Lighting::setAmbientColour() { ambientColour = lightColour * ambientStrength; }

void Lighting::setAmbientIntensity(const GLfloat pAmbientStrength) { ambientStrength = pAmbientStrength; }


// Diffuse Lighting 

void Lighting::setDiffuseColour() { diffuseColour = lightColour * diffuseStrength; }

void Lighting::setDiffuseIntensity(const GLfloat pDiffuseStrength) { diffuseStrength = pDiffuseStrength; }

const GLfloat Lighting::getDiffuseIntensity() const { return diffuseStrength; }

const glm::vec3 Lighting::getDiffuseColour() const { return diffuseColour; }

const glm::vec3 Lighting::getLightPosition() const { return lightPosition; }


// Specular Lighting 

const GLfloat Lighting::getSpecularIntensity() const { return specularStrength; }

const glm::vec3 Lighting::getSpecularColour() const { return specularColour; }

void Lighting::setSpecularIntensity(const GLfloat pSpecularStrength) { specularStrength = pSpecularStrength; }

void Lighting::setSpecularColour() { specularColour = lightColour * specularStrength; }


// ======================================== Directional Light ======================================================= //


DirectionalLight::DirectionalLight(const glm::vec3& pLightColor, const GLfloat pAmbientStrength, const GLfloat pDiffuseStrength, const GLfloat pSpecularStrength, const GLint pShininessValue)
{
	lightColour = pLightColor;
	ambientStrength = pAmbientStrength;
	diffuseStrength = pDiffuseStrength;
	specularStrength = pSpecularStrength;
	shininessValue = pShininessValue;
	lightDirection = -lightPosition;
}

// ======================================== Point Light ============================================================= //


PointLight::PointLight(const glm::vec3& pLightColor, const GLfloat pConstant, const GLfloat pLinear, const GLfloat pQuadratic, const GLfloat pAmbientStrength, const GLfloat pDiffuseStrength, const GLfloat pSpecularStrength, const GLint pShininessValue)
{
	lightColour = pLightColor;
	ambientStrength = pAmbientStrength;
	diffuseStrength = pDiffuseStrength;
	specularStrength = pSpecularStrength;
	shininessValue = pShininessValue;
	constant = pConstant;
	linear = pLinear;
	quadratic = pQuadratic;
}

const GLfloat PointLight::getConstant() const { return constant; }

const GLfloat PointLight::getLinear() const { return linear; }

const GLfloat PointLight::getQuadratic() const { return quadratic; }

void PointLight::setConstant(const GLfloat pConstant) { constant = pConstant; }

void PointLight::setLinear(const GLfloat pLinear) { linear = pLinear; }

void PointLight::setQuadratic(const GLfloat pQuadratic) { quadratic = pQuadratic; }


// ======================================== Spot Light ============================================================= //


SpotLight::SpotLight(const glm::vec3& pLightColor, const GLfloat pInnerCutOff, const GLfloat pOuterCutOff, const GLfloat pConstant, const GLfloat pLinear, const GLfloat pQuadratic, const GLfloat pAmbientStrength, const GLfloat pDiffuseStrength, const GLfloat pSpecularStrength, const GLint pShininessValue)
{
	lightColour = pLightColor;
	ambientStrength = pAmbientStrength;
	diffuseStrength = pDiffuseStrength;
	specularStrength = pSpecularStrength;
	shininessValue = pShininessValue;
	constant = pConstant;
	linear = pLinear;
	quadratic = pQuadratic;
	innerCutOff = pInnerCutOff;
	outerCutOff = pOuterCutOff; 
}

const GLfloat SpotLight::getInnerCutOff() const { return innerCutOff; }

const GLfloat SpotLight::getOuterCutOff() const { return outerCutOff; }

const GLfloat SpotLight::getConstant() const { return constant; }

const GLfloat SpotLight::getLinear() const { return linear; }

const GLfloat SpotLight::getQuadratic() const { return quadratic; }

void SpotLight::setInnerCutOff(const GLfloat pInnerCutOff) { innerCutOff = glm::cos(glm::radians(pInnerCutOff)); }

void SpotLight::setOuterCutOff(const GLfloat pOuterCutOff) { outerCutOff = glm::cos(glm::radians(pOuterCutOff)); }

void SpotLight::setConstant(const GLfloat pConstant) { constant = pConstant; }

void SpotLight::setLinear(const GLfloat pLinear) { linear = pLinear; }

void SpotLight::setQuadratic(const GLfloat pQuadratic) { quadratic = pQuadratic; }