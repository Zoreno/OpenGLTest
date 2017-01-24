#pragma once

#include <vector>
#include "PixelInfo.h"

class TextureFile
{
public:
	virtual ~TextureFile() = default;

	virtual const std::vector<uint8_t>& getPixels() const = 0;
	virtual uint32_t getWidth() const = 0;
	virtual uint32_t getHeight() const = 0;
	virtual bool hasAlpha() const = 0;
};