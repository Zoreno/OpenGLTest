#include "Texture2D.h"

#include "TGA.h"
#include "BMP.h"

#include <string>

Texture2D::Texture2D()
	:file{nullptr}, textureID{0} {}

Texture2D::Texture2D(const char* filePath)
{
	// TODO: Better way of detecting file format.

	size_t filePathLength = strlen(filePath);

	if(strcmp(filePath + filePathLength - 4, ".tga") == 0) // File is TGA
	{
		file = new TGA{ filePath };
	} 
	else if(strcmp(filePath + filePathLength - 4, ".bmp") == 0) // File is Bitmap
	{
		file = new BMP{ filePath };
	}
	else
	{
		throw std::invalid_argument(std::string("Invalid file format. (") + filePath + ")");
	}

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,							// Target
		0,										// Level
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Internal Format
		file->getWidth(),						// Width
		file->getHeight(),						// Height
		0,										// Border
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Format
		GL_UNSIGNED_BYTE,						// Type
		file->getPixels().data());				// Pixels

	// TODO: Make these parameters available to change
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::~Texture2D()
{
	if(file)
		delete file;

	if(textureID != 0)
		glDeleteTextures(1, &textureID);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	swap(*this, other);
}

uint32_t Texture2D::getWidth() const
{
	return file->getWidth();
}

uint32_t Texture2D::getHeight() const
{
	return file->getHeight();
}

GLuint Texture2D::getHandle() const
{
	return textureID;
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::bind(GLuint texUnit) const
{
	// TODO: Throw exception when texUnit is larger than supported number of units
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void swap(Texture2D& first, Texture2D& second) noexcept
{
	std::swap(first.textureID, second.textureID);
	std::swap(first.file, second.file);
}
