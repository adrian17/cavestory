#pragma once

#include "sprite.h"
#include "util\units.h"
#include <memory>
#include <vector>

class Graphics;

class NumberSprite
{
public:
	static NumberSprite HUDNumber(Graphics &graphics, int number, size_t nDigits)
		{return NumberSprite(graphics, number, nDigits, WHITE, NONE);}

	static NumberSprite DamageNumber(Graphics &graphics, int number)
		{return NumberSprite(graphics, number, 0, RED, MINUS); }

	static NumberSprite ExperienceNumber(Graphics &graphics, int number)
		{return NumberSprite(graphics, number, 0, WHITE, PLUS);}

	void draw(Graphics &graphics, Units::Game x, Units::Game y);
	void drawCentered(Graphics &graphics, Units::Game x, Units::Game y);

private:
	enum ColorType{ WHITE, RED };
	enum OperatorType { MINUS, PLUS, NONE };
	NumberSprite(Graphics &graphics, int number, size_t nDigits, ColorType color, OperatorType op);

	Units::Game width() const { return digits.size() * Units::halfTile; }
	Units::Game height() const { return Units::halfTile; }

	Units::Game padding = 0.0;
	std::vector<std::unique_ptr<Sprite>> digits;	//in reverse!
};

