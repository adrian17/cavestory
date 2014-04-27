#include "player.h"

#include "graphics.h"
#include "sprite/numberSprite.h"

namespace {
	const std::string spritePath = "content/TextBox.bmp";

	const Units::Game healthBarX = Units::tileToGame(1);
	const Units::Game healthBarY = Units::tileToGame(2);
	const Units::Pixel healthBarSourceX = 0;
	const Units::Pixel healthBarSourceY = Units::gameToPixel(5 * Units::halfTile);
	const Units::Pixel healthBarSourceW = Units::tileToPixel(4);
	const Units::Pixel healthBarSourceH = Units::gameToPixel(Units::halfTile);

	const Units::Game healthFillX = 5 * Units::halfTile;
	const Units::Game healthFillY = Units::tileToGame(2);
	const Units::Pixel healthFillSourceX = 0;
	const Units::Pixel healthFillSourceY = Units::gameToPixel(3 * Units::halfTile);
	//const Units::Pixel healthFillSourceW <- chagnes
	const Units::Pixel healthFillSourceH = Units::gameToPixel(Units::halfTile);

	const Units::Game healthNumberX = Units::tileToGame(3) / 2.0;
	const Units::Game healthNumberY = Units::tileToGame(2);
	const int numDigits = 2;

}

Player::Health::Health(Graphics &graphics) :
healthBarSprite(graphics, spritePath,
healthBarSourceX, healthBarSourceY,
healthBarSourceW, healthBarSourceH),
healthFillSprite(graphics, spritePath,
healthFillSourceX, healthFillSourceY,
Units::gameToPixel(5 * Units::halfTile - 2.0), healthFillSourceH)
{
}

void Player::Health::draw(Graphics &graphics){
	healthBarSprite.draw(graphics, healthBarX, healthBarY);
	healthFillSprite.draw(graphics, healthFillX, healthFillY);

	NumberSprite(graphics, 6, numDigits).draw(graphics, healthNumberX, healthNumberY);
}