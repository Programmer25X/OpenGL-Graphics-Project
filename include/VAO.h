#pragma once

#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>

#include"VBO.h"
#include "VertexBufferLayout.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject(); 

private:
	GLuint id;

public: 

	void addBuffer(const VertexBufferObject& VBO, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;
};


#endif