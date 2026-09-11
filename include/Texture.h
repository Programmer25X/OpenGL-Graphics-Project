#pragma once

#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& filePath, const bool gammaCorrection = false);
	~Texture();

private:
	GLuint ID = 0;
	std::string textureFilePath = "";
	std::string type = "";
	GLint width = 0;
	GLint height = 0;
	GLint numberOfColorChanels = 0;

public:
	const std::string getType() const;
	const GLuint getID() const; 
	void bind(const GLuint slot = 0) const; 
	void unbind() const; 
};

#endif