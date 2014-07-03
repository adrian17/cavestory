#pragma once

#include "util/units.h"
#include "SDL_rect.h"

class Graphics;
struct SDL_Texture;

class Sprite
{
public:
	Sprite(Graphics &graphics, const char* relativePath,
		Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height);
	virtual ~Sprite();

	virtual void update(){};
	void draw(Graphics &graphics, Units::Game x, Units::Game y) const;

protected:
	SDL_Rect sourceRect;

private:
	SDL_Texture *textureID;
};