#pragma once

#include "util/units.h"
#include <memory>

class Graphics;
class Sprite;

class FirstCaveBat
{
public:
	FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y);
	~FirstCaveBat();

	void update(Units::MS dt);
	void draw(Graphics &graphics) const;
private:
	Units::Game x, y;
	std::unique_ptr<Sprite> sprite;
};

