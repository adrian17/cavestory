#pragma once

#include "util\units.h"
#include <memory>
#include <vector>

class Graphics;
class Sprite;

class NumberSprite
{
public:
	NumberSprite(Graphics &graphics, int number, size_t nDigits = 0);

	void draw(Graphics &graphics, Units::Game x, Units::Game y);
private:
	Units::Game padding = 0.0;
	std::vector<std::unique_ptr<Sprite>> digits;	//in reverse!
};

