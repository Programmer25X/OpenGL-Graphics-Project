#pragma once

#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VertexBufferObject
{
public:
	VertexBufferObject(const void* vertices, GLuint size);
	~VertexBufferObject(); 

private:
	GLuint id; 

public:
	void bind() const;
	void unbind() const;
};

#endif
