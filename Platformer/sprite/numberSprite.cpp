#include "numberSprite.h"

#include "graphics.h"
#include "sprite.h"

namespace {
	const std::string spritePath = "content/TextBox.bmp";
	const Units::Pixel sourceY = Units::gameToPixel(7 * Units::halfTile);
	const Units::Pixel sourceW = Units::gameToPixel(Units::halfTile);
	const Units::Pixel sourceH = Units::gameToPixel(Units::halfTile);
}

NumberSprite::NumberSprite(Graphics &graphics, int number, size_t nDigits)
{
	_ASSERT(number >= 0);
	do{
		const int digit = number % 10;
		digits.push_back(std::unique_ptr<Sprite>(new Sprite(graphics, spritePath,
			Units::gameToPixel(digit * Units::halfTile), sourceY,
			sourceW, sourceH)));
		number /= 10;
	} while (number != 0);
	_ASSERT(nDigits == 0 || nDigits >= digits.size());
	padding = (nDigits == 0) ? 0.0 : (nDigits - digits.size()) * Units::halfTile;
}

void NumberSprite::draw(Graphics &graphics, Units::Game x, Units::Game y){
	Units::Game offset = Units::halfTile * (digits.size() - 1);	//move from right to left
	for (auto &&sprite : digits){
		sprite->draw(graphics, x + offset + padding, y);
		offset -= Units::halfTile;
	}
}