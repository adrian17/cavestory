#pragma once

#include "util\units.h"
#include <memory>

class Graphics;
class Sprite;

class NumberSprite
{
public:
	NumberSprite(Graphics &graphics, int number);

	void draw(Graphics &graphics, Units::Game x, Units::Game y);
private:
	std::unique_ptr<Sprite> sprite;
};

