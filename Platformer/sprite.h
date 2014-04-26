#pragma once

#include <string>
#include "SDL.h"

class Graphics;

class Sprite
{
public:
	Sprite(Graphics &graphics, const std::string filePath,
		int srcX, int srcY, int width, int height);
	virtual ~Sprite();

	virtual void Update(int dt){};
	void Draw(Graphics &graphics, int x, int y);

protected:
	SDL_Rect sourceRect;

private:
	SDL_Texture *spriteSheet;
};