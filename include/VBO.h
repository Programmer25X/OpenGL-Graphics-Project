#pragma once

#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VertexBufferObject
{
public:
	VertexBufferObject(); 
	VertexBufferObject(const void* vertices, GLsizeiptr size);
	~VertexBufferObject(); 

private:
	GLuint id = 0; 

public:
	void bind() const;
	void unbind() const;
};

#endif
