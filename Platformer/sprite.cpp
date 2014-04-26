#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(Graphics &graphics, const std::string filePath, 
	int srcX, int srcY, int width, int height)
{
	spriteSheet = graphics.createTexture(filePath);
	sourceRect.x = srcX; sourceRect.y = srcY;
	sourceRect.w = width; sourceRect.h = height;
}

Sprite::~Sprite(){
}

void Sprite::draw(Graphics &graphics, int x, int y){
	SDL_Rect destRect = { x, y, sourceRect.w, sourceRect.h };
	graphics.drawTexture(spriteSheet, &sourceRect, &destRect);
}