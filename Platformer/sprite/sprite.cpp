#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(Graphics &graphics, const char* relativePath,
	Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height)
{
	const bool transparency = true;
	textureID = graphics.createTexture(relativePath, transparency);
	sourceRect.x = srcX; sourceRect.y = srcY;
	sourceRect.w = width; sourceRect.h = height;
}

Sprite::~Sprite(){
}

void Sprite::draw(Graphics &graphics, Units::Game x, Units::Game y) const{
	SDL_Rect destRect = { Units::gameToPixel(x), Units::gameToPixel(y), sourceRect.w, sourceRect.h };
	graphics.drawTexture(textureID, &sourceRect, &destRect);
}