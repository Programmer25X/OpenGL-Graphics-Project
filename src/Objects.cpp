#include "Objects.h"

#include <cmath>
#include <algorithm>

Object::Object()
{
	shininess = std::ranges::clamp(static_cast<GLint>(shininess), 1, 256); 
}

const glm::vec3 Object::getPosition() const { return position; }

const GLuint Object::getShininess() const { return shininess; }

void Object::setShininess(GLuint pShininess) { shininess = std::ranges::clamp(static_cast<GLint>(pShininess), 1, 256); }

const std::vector<GLuint> Object::getIndices() const { return indices; }

const std::vector<GLfloat> Object::getVerticies() const { return verticies; }



BasicCube::BasicCube()
{
	verticies =
	{
		// Coordinates			  Normals	           Texture Coordinates 

	-100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
	 100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	-100.0f,  100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,

	-100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-100.0f,  100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

	-100.0f,  100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-100.0f, -100.0f, -100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-100.0f, -100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,		-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 100.0f,  100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	 100.0f,  100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

	-100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	 100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	 100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	-100.0f, -100.0f,  100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	-100.0f, -100.0f, -100.0f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

	-100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	 100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	 100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	 100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	-100.0f,  100.0f,  100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	-100.0f,  100.0f, -100.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f

	};

	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	position = glm::vec3(0.0f, 0.0f, 0.0f);

}
