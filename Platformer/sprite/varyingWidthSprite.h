#pragma once

#include "sprite.h"

class VaryingWidthSprite : public Sprite{
public:
	VaryingWidthSprite(Graphics &graphics, const std::string relativePath,
		Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height) :
		Sprite(graphics, relativePath, srcX, srcY, width, height)
	{

	}
	void setWidth(Units::Pixel width){
		sourceRect.w = width;
	}
};