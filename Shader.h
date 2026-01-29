#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragementFile);
	~Shader(); 
	

private:
	GLuint id = 0; 
	std::string vertexInfomation = "";
	std::string fragmentInfomation = "";
	std::unordered_map<std::string, int> uniformLocationCache; 


public:
	void compileShader();
	void useShader() const; 
	void stopUsingShader() const; 

	const GLint getUniformLocation(const std::string& uniformName);

	void setUniform1i(const std::string& uniformName, GLint value);
	void setUniform1f(const std::string& uniformName, GLfloat value);
	void setUniformMatrix4f(const std::string& uniformName, const glm::mat4& matrix);
	void setVector4(const std::string& uniformName, const GLfloat parameterOne, const GLfloat parameterTwo, const GLfloat perameterThree, const GLfloat parameterFour);

};

#endif

