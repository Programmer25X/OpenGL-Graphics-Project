#pragma once

#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>
#include <stdexcept>


// Attributes per Vertex
struct VertexBufferElement 
{
	GLuint type = 0;
	GLuint count = 0;
	GLboolean isNormalised; 
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout(); 

private:
	std::vector<VertexBufferElement> elements; 
	GLsizei stride = 0; 

public:

	template <typename T>
	void pushElement(GLuint countParameter, GLboolean normlaise = GL_FALSE)
	{
		throw std::runtime_error("ERROR::UNSUPPORTED TYPE PUSHED TO VERTEX BUFFER LAYOUT");
	}

	template<>
	void pushElement<GLfloat>(GLuint countParameter, GLboolean normlaise)
	{
		elements.push_back({ GL_FLOAT, countParameter, normlaise});
		stride += countParameter * VertexBufferLayout::calculateSize(GL_FLOAT);
	}

	template<>
	void pushElement<GLuint>(GLuint countParameter, GLboolean normlaise)
	{
		elements.push_back({ GL_UNSIGNED_INT, countParameter, normlaise});
		stride += countParameter * VertexBufferLayout::calculateSize(GL_UNSIGNED_INT);
	}

	template<>
	void pushElement<GLboolean>(GLuint countParameter, GLboolean normlaise)
	{
		elements.push_back({ GL_BOOL, countParameter, normlaise});
		stride += countParameter * VertexBufferLayout::calculateSize(GL_BOOL);

	}

	 const std::vector<VertexBufferElement>& getElements() const;
	 const GLsizei getStride() const;
	 static GLint calculateSize(GLuint type);
};


#endif


