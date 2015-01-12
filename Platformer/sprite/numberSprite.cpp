#include "numberSprite.h"

#include <cassert>

namespace {
	const char* spritePath = "TextBox.bmp";
	const Units::Pixel sourceWhiteY = Units::gameToPixel(7 * Units::halfTile);
	const Units::Pixel sourceRedY = Units::gameToPixel(8 * Units::halfTile);
	const Units::Pixel sourceW = Units::gameToPixel(Units::halfTile);
	const Units::Pixel sourceH = Units::gameToPixel(Units::halfTile);

	const Units::Pixel opPlusSourceX = Units::gameToPixel(4 * Units::halfTile);
	const Units::Pixel opMinusSourceX = Units::gameToPixel(5 * Units::halfTile);
	const Units::Pixel opSourceY = Units::gameToPixel(6 * Units::halfTile);
}

NumberSprite::NumberSprite(Graphics &graphics, int number, size_t nDigits, ColorType color, OperatorType op)
{
	Units::Pixel sourceY = (color == WHITE) ? sourceWhiteY : sourceRedY;
	assert(number >= 0);
	do{
		const int digit = number % 10;
		digits.push_back(std::unique_ptr<Sprite>(new Sprite(graphics, spritePath,
			Units::gameToPixel(digit * Units::halfTile), sourceY,
			sourceW, sourceH)));
		number /= 10;
	} while (number != 0);

	if (op == PLUS) digits.push_back(std::unique_ptr<Sprite>(new Sprite(graphics, spritePath,
		opPlusSourceX, opSourceY, sourceW, sourceH)));
	else if (op == MINUS) digits.push_back(std::unique_ptr<Sprite>(new Sprite(graphics, spritePath,
		opMinusSourceX, opSourceY, sourceW, sourceH)));

	assert(nDigits == 0 || nDigits >= digits.size());
	padding = (nDigits == 0) ? 0.0 : (nDigits - digits.size()) * Units::halfTile;
}

void NumberSprite::draw(Graphics &graphics, Units::Game x, Units::Game y) const{
	Units::Game offset = Units::halfTile * (digits.size() - 1);	//move from right to left
	for (auto &&sprite : digits){
		sprite->draw(graphics, x + offset + padding, y);
		offset -= Units::halfTile;
	}
}

void NumberSprite::drawCentered(Graphics &graphics, Units::Game x, Units::Game y) const{
	draw(graphics, x - width() / 2, y - height() / 2);
}