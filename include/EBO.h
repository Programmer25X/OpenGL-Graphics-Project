#pragma once

#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class ElementBufferObject
{

public:
	ElementBufferObject();
	ElementBufferObject(const GLuint* indices, const GLuint count); 
	~ElementBufferObject();

private:
	GLuint id = 0;
	GLuint elementCount = 0; 

public:
	void bind() const;
	void unbind() const;

	const GLuint getElementCount() const; 
};

#endif
