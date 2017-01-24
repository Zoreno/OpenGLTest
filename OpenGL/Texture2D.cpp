#include "Texture2D.h"

#include "TGA.h"
#include "BMP.h"

#include <string>

Texture2D::Texture2D(const char* filePath)
{
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
	delete file;

	glDeleteTextures(1, &textureID);
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
