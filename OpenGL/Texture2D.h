#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureFile.h"

class Texture2D
{
public:
	explicit Texture2D(const char* filePath);
	~Texture2D();

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	GLuint getHandle() const;

	void bind() const;
private:
	TextureFile* file;

	GLuint textureID;
};