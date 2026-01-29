#include "VAO.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArrayObject::VertexArrayObject()
{
	LOG_ERRORS(glGenVertexArrays(1, &id));
}

VertexArrayObject::~VertexArrayObject()
{
	LOG_ERRORS(glDeleteVertexArrays(1, &id));
}

void VertexArrayObject::addBuffer(const VertexBufferObject& VBO, const VertexBufferLayout& layout) const
{
	LOG_ERRORS(bind()); 
	LOG_ERRORS(VBO.bind());

	const auto& elements = layout.getElements();
	GLuint offset = 0;

	for (GLuint currentElement = 0; currentElement < elements.size(); currentElement++)
	{
		const auto& element = elements[currentElement];
		GLsizei stride = layout.getStride();
		LOG_ERRORS(glEnableVertexAttribArray(currentElement));
		LOG_ERRORS(glVertexAttribPointer(currentElement, element.count, element.type, element.isNormalised, stride, (const void*)offset));
		offset += element.count * VertexBufferLayout::calculateSize(element.type);
	}
}

void VertexArrayObject::bind() const
{
	LOG_ERRORS(glBindVertexArray(id));
}

void VertexArrayObject::unbind() const
{
	LOG_ERRORS(glBindVertexArray(0));
}

