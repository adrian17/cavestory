#pragma once

#include <string>
#include "SDL.h"

class Graphics;

class Sprite
{
public:
	Sprite(Graphics &graphics, const std::string filePath, int srcX, int srcY, int width, int height);
	~Sprite();

	void Draw(Graphics &graphics, int x, int y);
private:
	SDL_Texture *spriteSheet;
	SDL_Rect sourceRect;
};

