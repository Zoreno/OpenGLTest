#pragma once

#include <vector>
#include "TextureFile.h"

class BMP : public TextureFile
{
public:
	explicit BMP(const char* filePath);

	const std::vector<uint8_t>& getPixels() const override;
	uint32_t getWidth() const override;
	uint32_t getHeight() const override;
	bool hasAlpha() const override;

private:
	uint32_t width;
	uint32_t height;
	uint16_t bitsPerPixel;
	std::vector<uint8_t> pixels;
};