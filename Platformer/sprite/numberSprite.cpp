#include "numberSprite.h"

#include "graphics.h"
#include "sprite.h"

NumberSprite::NumberSprite(Graphics &graphics, int number)
{
	sprite.reset(new Sprite(graphics, "content/TextBox.bmp",
		Units::gameToPixel(number * Units::halfTile), Units::gameToPixel(7 * Units::halfTile),
		Units::gameToPixel(Units::halfTile), Units::gameToPixel(Units::halfTile)));
}

void NumberSprite::draw(Graphics &graphics, Units::Game x, Units::Game y){
	sprite->draw(graphics, x, y);
}