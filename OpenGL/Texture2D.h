#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureFile.h"

#include <map>

class Texture2D
{
public:

	Texture2D();

	explicit Texture2D(const char* filePath);
	~Texture2D();

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	Texture2D(Texture2D&& other) noexcept;

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	GLuint getHandle() const;

	// Bind texture to currently active unit
	void bind() const;

	// Bind texture to given unit
	void bind(GLuint texUnit) const;
private:
	TextureFile* file;

	GLuint textureID;

	friend void swap(Texture2D& first, Texture2D& second) noexcept;
};

typedef std::map<std::string, Texture2D*> TextureMap;

