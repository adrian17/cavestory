#pragma once

#include "sprite.h"
#include <cassert>

class VaryingWidthSprite : public Sprite{
public:
	VaryingWidthSprite(Graphics &graphics, const char* relativePath,
		Units::Pixel srcX, Units::Pixel srcY,
		Units::Pixel maxWidth,
		Units::Pixel width, Units::Pixel height) :
		Sprite(graphics, relativePath, srcX, srcY, width, height),
		maxWidth(maxWidth)
	{

	}
	void setPercentageWidth(double percentage){
		assert(percentage <= 1.0 && percentage >= 0.0);
		sourceRect.w = int(percentage * maxWidth);
	}
private:
	const Units::Pixel maxWidth;
};