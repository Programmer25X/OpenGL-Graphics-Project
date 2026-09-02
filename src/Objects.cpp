#include "Objects.h"

const glm::vec3 BasicCube::getCubePosition() const { return cubePosition; }

const std::vector<GLuint> BasicCube::getIndices() const { return indices; }

const std::vector<GLfloat> BasicCube::getVerticies() const { return verticies; }
