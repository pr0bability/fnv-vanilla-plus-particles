#pragma once

#include "BSRenderedTexture.hpp"

class ImageSpaceTexture {
public:
	ImageSpaceTexture();
	~ImageSpaceTexture();

	bool				byte0;
	bool				bIsRenderedTexture;
	bool				bIsBorrowed;
	NiObjectPtr			spTexture;
	uint32_t			iFilterMode;
	uint32_t			iClampMode;
};

ASSERT_SIZE(ImageSpaceTexture, 0x10);
