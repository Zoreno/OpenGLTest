#pragma once

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


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

	// Add more as needed

	void uploadUniform(const std::string& name, float value);
	void uploadUniform(const std::string& name, int value);
	void uploadUniform(const std::string& name, glm::vec2 value);
	void uploadUniform(const std::string& name, glm::vec3 value);
	void uploadUniform(const std::string& name, glm::vec4 value);
	void uploadUniform(const std::string& name, glm::mat4 value);
private:

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	GLuint shaderProgramHandle;
	GLuint vertexShaderHandle;
	GLuint fragmentShaderHandle;
};

