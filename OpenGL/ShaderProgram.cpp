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
	if (glIsShader(vertexShaderHandle))
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
		errorMessage = "Error compiling vertex shader (" + vertexShaderPath + ")\n" + infoLog;
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
	if (glIsProgram(shaderProgramHandle))
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

void ShaderProgram::uploadUniform(const std::string& name, float value)
{
	use();
	glUniform1f(glGetUniformLocation(shaderProgramHandle, name.c_str()), value);
}

void ShaderProgram::uploadUniform(const std::string& name, int value)
{
	use();
	glUniform1i(glGetUniformLocation(shaderProgramHandle, name.c_str()), value);
}

void ShaderProgram::uploadUniform(const std::string& name, glm::vec2 value)
{
	use();
	glUniform2f(glGetUniformLocation(shaderProgramHandle, name.c_str()), value.x, value.y);
}

void ShaderProgram::uploadUniform(const std::string& name, glm::vec3 value)
{
	use();
	glUniform3f(glGetUniformLocation(shaderProgramHandle, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::uploadUniform(const std::string& name, glm::vec4 value)
{
	use();
	glUniform4f(glGetUniformLocation(shaderProgramHandle, name.c_str()), value.x, value.y, value.z, value.w);
}

void ShaderProgram::uploadUniform(const std::string& name, glm::mat4 value)
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void swap(ShaderProgram& lhs, ShaderProgram& rhs) noexcept
{

	using std::swap;
	swap(lhs.vertexShaderPath, rhs.vertexShaderPath);
	swap(lhs.fragmentShaderPath, rhs.fragmentShaderPath);
	swap(lhs.shaderProgramHandle, rhs.shaderProgramHandle);
	swap(lhs.vertexShaderHandle, rhs.vertexShaderHandle);
	swap(lhs.fragmentShaderHandle, rhs.fragmentShaderHandle);

}
