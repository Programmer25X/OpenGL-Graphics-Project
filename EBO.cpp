#include "EBO.h"
#include "Renderer.h"

ElementBufferObject::ElementBufferObject()
{
}

ElementBufferObject::ElementBufferObject(const GLuint* indices, const GLuint count)
{
	elementCount = count;

	ASSERT(sizeof(unsigned int) == sizeof(GLuint))
	LOG_ERRORS(glGenBuffers(1, &id));
	LOG_ERRORS(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	LOG_ERRORS(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW));
}

ElementBufferObject::~ElementBufferObject()
{
	LOG_ERRORS(glDeleteBuffers(1, &id));
}

void ElementBufferObject::bind() const
{
	LOG_ERRORS(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void ElementBufferObject::unbind() const
{
	LOG_ERRORS(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

const GLuint ElementBufferObject::getElementCount() const
{
	return elementCount;
}
