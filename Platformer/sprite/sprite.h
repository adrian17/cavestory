#pragma once

#include "util/units.h"
#include <string>
#include "SDL.h"

class Graphics;

class Sprite
{
public:
	Sprite(Graphics &graphics, const std::string &relativePath,
		Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height);
	virtual ~Sprite();

	virtual void update(){};
	void draw(Graphics &graphics, Units::Game x, Units::Game y);

protected:
	SDL_Rect sourceRect;

private:
	SDL_Texture *spriteSheet;
};