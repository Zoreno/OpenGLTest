#include "ShaderProgram.h"

#include "Utils.h"

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	:vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath)
{}

ShaderProgram::~ShaderProgram()
{
	if (glIsProgram(shaderProgramHandle))
	{
		glDeleteProgram(shaderProgramHandle);
	}

	if (glIsShader(vertexShaderHandle))
	{
		glDeleteShader(vertexShaderHandle);
	}

	if (glIsShader(fragmentShaderHandle))
	{
		glDeleteShader(fragmentShaderHandle);
	}
}

void ShaderProgram::compile()
{
	if(glIsShader(vertexShaderHandle))
	{
		glDeleteShader(vertexShaderHandle);
	}

	if (glIsShader(fragmentShaderHandle))
	{
		glDeleteShader(fragmentShaderHandle);
	}

	// Create a new vertex shader
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderString = getStringFromFile(vertexShaderPath);
	const char* vertexShaderSource = vertexShaderString.c_str();
	glShaderSource(vertexShaderHandle, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderHandle);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderHandle, 512, NULL, infoLog);
		std::string errorMessage;
		errorMessage =  "Error compiling vertex shader (" + vertexShaderPath + ")\n" + infoLog;
		throw ShaderProgramException(errorMessage);
	}

	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragmentShaderString = getStringFromFile(fragmentShaderPath);
	const char* fragmentShaderSource = fragmentShaderString.c_str();
	glShaderSource(fragmentShaderHandle, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderHandle);

	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderHandle, 512, NULL, infoLog);
		std::string errorMessage;
		errorMessage = "Error compiling fragment shader (" + fragmentShaderPath + ")\n" + infoLog;
		throw ShaderProgramException(errorMessage);
	}

	// If the prorgram is already existing, delete it before creating a new one.
	if(glIsProgram(shaderProgramHandle))
	{
		glDeleteProgram(shaderProgramHandle);
	}

	shaderProgramHandle = glCreateProgram();

	glAttachShader(shaderProgramHandle, vertexShaderHandle);
	glAttachShader(shaderProgramHandle, fragmentShaderHandle);

}

void ShaderProgram::link() const
{
	GLint success;
	GLchar infoLog[512];
	glLinkProgram(shaderProgramHandle);

	glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramHandle, 512, NULL, infoLog);
		std::string errorMessage;
		errorMessage = std::string{ "Error linking shader program\n" } +std::string{ infoLog };
		throw ShaderProgramException(errorMessage);
	}
}

void ShaderProgram::bindAttribLocation(GLuint index, const GLchar* name) const
{
	glBindAttribLocation(shaderProgramHandle, index, name);
}

void ShaderProgram::use() const
{
	glUseProgram(shaderProgramHandle);
}

void ShaderProgram::disable() const
{
	glUseProgram(0);
}

GLuint ShaderProgram::getShaderProgramHandle() const
{
	return shaderProgramHandle;
}
