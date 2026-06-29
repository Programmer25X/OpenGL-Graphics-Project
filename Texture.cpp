#include "Texture.h"
#include "stb_image.h"

#include <iostream>



Texture::Texture(const std::string& filePath)
{
	id = 0;
	textureFilePath = filePath;
	buffer = nullptr;
	width = 0;
	height = 0;
	numberOfColorChanels = 0; 

	LOG_ERRORS(glGenTextures(1, &id));
	bind();

	// Setting Texture Filtering options 
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR));

	// Setting Texture Wrapping options
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(filePath.c_str(), &width, &height, &numberOfColorChanels, 4); 

	if (buffer != nullptr)
	{
		LOG_ERRORS(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(buffer)));
		LOG_ERRORS(glGenerateMipmap);
		unbind();
	}
	else
	{
		std::cerr << "ERROR::FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(buffer);
}

Texture::~Texture()
{
	LOG_ERRORS(glDeleteTextures(1, &id)); 
}

const std::string Texture::getType() const
{
	return type;
}

const GLuint Texture::getId() const
{
	return id; 
}

void Texture::bind(GLuint slot) const
{
	LOG_ERRORS(glActiveTexture(GL_TEXTURE0 + slot)); 
	LOG_ERRORS(glBindTexture(GL_TEXTURE_2D, id))
}

void Texture::unbind() const
{
	LOG_ERRORS(glBindTexture(GL_TEXTURE_2D, 0))

}
