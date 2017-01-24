#pragma once

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class ShaderProgramException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class ShaderProgram
{
public:
	ShaderProgram() = delete;
	ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~ShaderProgram();

	void compile();
	void link() const;

	void bindAttribLocation(GLuint index, const GLchar* name) const;

	void use() const;
	void disable() const;

	GLuint getShaderProgramHandle() const;
private:

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	GLuint shaderProgramHandle;
	GLuint vertexShaderHandle;
	GLuint fragmentShaderHandle;
};

