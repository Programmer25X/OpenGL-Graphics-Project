#include "Texture.h"
#include "stb/stb_image.h"

#include <iostream>



Texture::Texture(const std::string& filePath, const bool gammaCorrection)
{
	ID = 0;
	textureFilePath = filePath;
	width = 0;
	height = 0;
	numberOfColorChanels = 0; 

	unsigned char* buffer = nullptr;

	LOG_ERRORS(glGenTextures(1, &ID));
	LOG_ERRORS(bind());

	// Setting Texture Filtering options 
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR));

	// Setting Texture Wrapping options
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	LOG_ERRORS(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(filePath.c_str(), &width, &height, &numberOfColorChanels, 0); 

	if (buffer != nullptr)
	{
		GLenum internalFormat = {};
		GLenum dataFormat = {};

		switch (numberOfColorChanels)
		{
		case 1:
			internalFormat = dataFormat = GL_RED;
			break;
		case 3:
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
			break;
		case 4:
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
			break;
		default:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA; 
			break;
		}

		LOG_ERRORS(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(buffer)));
		LOG_ERRORS(glGenerateMipmap(GL_TEXTURE_2D));
		LOG_ERRORS(unbind());
	}
	else
	{
		std::cerr << "ERROR::FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(buffer);
}

Texture::~Texture()
{
	LOG_ERRORS(glDeleteTextures(1, &ID)); 
}

const std::string Texture::getType() const
{
	return type;
}

const GLuint Texture::getID() const
{
	return ID; 
}

void Texture::bind(GLuint slot) const
{
	LOG_ERRORS(glActiveTexture(GL_TEXTURE0 + slot)); 
	LOG_ERRORS(glBindTexture(GL_TEXTURE_2D, ID))
}

void Texture::unbind() const
{
	LOG_ERRORS(glBindTexture(GL_TEXTURE_2D, 0))

}
