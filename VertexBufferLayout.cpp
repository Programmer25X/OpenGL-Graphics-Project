#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
{

}

VertexBufferLayout::~VertexBufferLayout()
{

}

const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const { return elements; }

const GLsizei VertexBufferLayout::getStride() const { return stride; }

GLint VertexBufferLayout::calculateSize(GLuint type)
{
	GLuint returnValue = 0;

	switch (type)
	{
		case(GL_UNSIGNED_INT):
			 returnValue = sizeof(GL_UNSIGNED_INT);
			 break;
		case(GL_INT):
			returnValue = sizeof(GL_INT);
			break;
		case(GL_FLOAT):
			returnValue = sizeof(GL_FLOAT);
			break;
		case(GL_BOOL):
			returnValue = sizeof(GL_BOOL);
			break;
		default:
			break;
	}

	return returnValue;
}
