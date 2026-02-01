#include "VBO.h"
#include "Renderer.h"

#include <ctype.h>

VertexBufferObject::VertexBufferObject(const void* vertices, GLsizeiptr size)
{
	LOG_ERRORS(glGenBuffers(1, &id));
	LOG_ERRORS(glBindBuffer(GL_ARRAY_BUFFER, id));
	LOG_ERRORS(glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vertices, GL_STATIC_DRAW));
}

VertexBufferObject::~VertexBufferObject()
{
	LOG_ERRORS(glDeleteBuffers(1, &id));
}

void VertexBufferObject::bind() const
{
	LOG_ERRORS(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBufferObject::unbind() const
{
	LOG_ERRORS(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

