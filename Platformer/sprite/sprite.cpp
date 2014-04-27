#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(Graphics &graphics, const std::string filePath, 
	Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height)
{
	const bool transparency = true;
	spriteSheet = graphics.createTexture(filePath, transparency);
	sourceRect.x = srcX; sourceRect.y = srcY;
	sourceRect.w = width; sourceRect.h = height;
}

Sprite::~Sprite(){
}

void Sprite::draw(Graphics &graphics, Units::Game x, Units::Game y){
	SDL_Rect destRect = { Units::gameToPixel(x), Units::gameToPixel(y), sourceRect.w, sourceRect.h };
	graphics.drawTexture(spriteSheet, &sourceRect, &destRect);
}