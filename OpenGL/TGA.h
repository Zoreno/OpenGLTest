#pragma once

#include "TextureFile.h"

class TGA : public TextureFile
{
public:
	explicit TGA(const char* filePath);

	const std::vector<uint8_t>& getPixels() const override;
	uint32_t getWidth() const override;
	uint32_t getHeight() const override;
	bool hasAlpha() const override;

private:
	std::vector<uint8_t> pixels;
	bool isCompressed;
	uint32_t width;
	uint32_t height;
	uint32_t size;
	uint32_t bitsPerPixel;
};