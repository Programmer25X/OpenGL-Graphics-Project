#pragma once

#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class ElementBufferObject
{

public:
	ElementBufferObject(const GLuint* indices, const GLuint count); 
	~ElementBufferObject();

private:
	GLuint id;
	GLuint elementCount; 

public:
	void bind() const;
	void unbind() const;

	const GLuint getElementCount() const; 
};

#endif
