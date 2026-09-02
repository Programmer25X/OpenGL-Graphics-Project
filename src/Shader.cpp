#include "Shader.h"
#include "Renderer.h"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<format>


Shader::Shader(const char* vertexFile, const char* fragementFile)
{
	vertexInfomation = vertexFile;
	fragmentInfomation = fragementFile; 
	compileShader(); 
}

Shader::~Shader()
{
	if (id != 0)
	{
		LOG_ERRORS(glDeleteProgram(id));
	}
}

void Shader::compileShader()
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexInfomation); // Opens the vertex file
		fragmentShaderFile.open(fragmentInfomation); // Opnes the fragment file

		std::stringstream vertexShaderStream, fragmentShaderStream;

		if (vertexShaderFile.is_open()) // Checks if the vertex file is open
		{
			vertexShaderStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();
		}

		if (fragmentShaderFile.is_open()) // Checks if the fragment file is open
		{
			fragmentShaderStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();
		}

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (const std::ifstream::failure error)
	{
		std::cerr << std::format("ERROR::SHADER::FILE_UNSUCCESSFULLY_READ\n{}", error.what()) << std::endl;
	}

	const GLchar* vertexSource = vertexCode.c_str();
	const GLchar* fragmentSource = fragmentCode.c_str();

	GLchar infoLog[512];


	// VERTEX SHADER

	GLint vertexShaderSuccess = 0;
	LOG_ERRORS(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
	LOG_ERRORS(glShaderSource(vertexShader, 1, &vertexSource, NULL));
	LOG_ERRORS(glCompileShader(vertexShader));
	LOG_ERRORS(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess)); // Gets whether the vertex shader linked correctly

	if (!vertexShaderSuccess)
	{
		LOG_ERRORS(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
		std::cerr << std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog) << std::endl; // Outputs the error
		LOG_ERRORS(glDeleteShader(vertexShader));
		return; 
	}


	// FRAGMENT SHADER

	GLint fragmentShaderSuccess = 0;
	LOG_ERRORS(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	LOG_ERRORS(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
	LOG_ERRORS(glCompileShader(fragmentShader));
	LOG_ERRORS(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess)); // Gets whether the fragment shader linked correctly

	if (fragmentShaderSuccess == GL_FALSE)
	{
		LOG_ERRORS(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
		std::cerr << std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog) << std::endl; // Outputs the error
		LOG_ERRORS(glDeleteShader(fragmentShader));
		return;
	}


	GLint shaderSuccess = 0;
	id = glCreateProgram();
	LOG_ERRORS(glAttachShader(id, vertexShader));
	LOG_ERRORS(glAttachShader(id, fragmentShader));
	LOG_ERRORS(glLinkProgram(id));
	LOG_ERRORS(glGetProgramiv(id, GL_LINK_STATUS, &shaderSuccess)); // Gets whether the link was successful 
	LOG_ERRORS(glValidateProgram(id));  // Checks whether executables within the program can execute, depending on OpenGL state

	if (shaderSuccess == GL_FALSE)
	{
		LOG_ERRORS(glGetProgramInfoLog(id, 512, NULL, infoLog));
		std::cerr << std::format("ERROR::SHADER::LINK_FAILED\n{}", infoLog) << std::endl; // Outputs the error
		LOG_ERRORS(glDeleteProgram(id));
		return;
	}

	LOG_ERRORS(glDeleteShader(vertexShader));
	LOG_ERRORS(glDeleteShader(fragmentShader));
}

void Shader::useShader() const
{
	LOG_ERRORS(glUseProgram(id));
}

void Shader::stopUsingShader() const
{
	LOG_ERRORS(glUseProgram(0));  
}


void Shader::setUniform1i(const std::string& uniformName, GLint value)
{
	LOG_ERRORS(glUniform1i(getUniformLocation(uniformName), value))
}

void Shader::setUniform1f(const std::string& uniformName, GLfloat value) 
{
	LOG_ERRORS(glUniform1f(getUniformLocation(uniformName), value));
}

void Shader::setUniformMatrix4f(const std::string& uniformName, const glm::mat4& matrix)
{
	LOG_ERRORS(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniformVector3(const std::string& uniformName, const GLfloat parameterOne, const GLfloat parameterTwo, const GLfloat perameterThree)
{
	LOG_ERRORS(glUniform3f(getUniformLocation(uniformName), parameterOne, parameterTwo, perameterThree));
}

void Shader::setUniformVector4(const std::string& uniformName, const GLfloat parameterOne, const GLfloat parameterTwo, const GLfloat parameterThree, const GLfloat parameterFour)
{
	LOG_ERRORS(glUniform4f(getUniformLocation(uniformName), parameterOne, parameterTwo, parameterThree, parameterFour));
}

void Shader::setUniformBoolean(const std::string& uniformName, const bool value)
{
	GLuint passedValue = 0;

	if (value == GL_TRUE || value == true)
	{
		passedValue = 1; // True
	}
	else
	{
		passedValue = 0;
	}

	LOG_ERRORS(glUniform1i(getUniformLocation(uniformName), passedValue));
}

const GLint Shader::getUniformLocation(const std::string& uniformName)
{
	// Caching the Uniform

	if (uniformLocationCache.find(uniformName) != uniformLocationCache.end())
	{
		return static_cast<GLint>(uniformLocationCache[uniformName]);
	}
	else
	{
		LOG_ERRORS(GLint location = glGetUniformLocation(id, uniformName.c_str()));

		if (location == -1)
		{
			std::clog << "WARNING:: UNIFORM " << location << " DOES NOT EXIST" << std::endl;
		}			


		uniformLocationCache[uniformName] = location;
		return static_cast<GLint>(uniformLocationCache[uniformName]);
	}
}



