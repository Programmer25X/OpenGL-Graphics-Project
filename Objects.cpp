#include "Objects.h"

const glm::vec3 LogoCube::getCubePosition() const { return cubePosition; }

const std::vector<GLuint> LogoCube::getIndices() const { return indices; }

const std::vector<GLfloat> LogoCube::getVerticies() const { return verticies; }
