#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(Graphics &graphics, const std::string filePath, int srcX, int srcY, int width, int height)
{
	spriteSheet = graphics.CreateTexture(filePath);
	sourceRect.x = srcX; sourceRect.y = srcY;
	sourceRect.w = width; sourceRect.h = height;
}


Sprite::~Sprite()
{
	SDL_DestroyTexture(spriteSheet);
}

void Sprite::Draw(Graphics &graphics, int x, int y){
	SDL_Rect destRect = { x, y, sourceRect.w, sourceRect.h };
	graphics.DrawTexture(spriteSheet, &sourceRect, &destRect);
}