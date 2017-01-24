#pragma once

#include <cstdint>

typedef union PixelInfo
{
	uint32_t color;

	struct
	{
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t A;
	};
} *PixelInfoPtr;