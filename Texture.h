#pragma once

#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

private:
	GLuint id = 0;
	std::string textureFilePath = "";
	unsigned char* buffer = nullptr;
	GLint width = 0;
	GLint height = 0;
	GLint numberOfColorChanels = 0;

public:
	void bind(GLuint slot = 0) const; 
	void unbind() const; 
};

#endif