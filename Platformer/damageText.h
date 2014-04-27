#pragma once

#include "util\timer.h"
#include "util\units.h"

class Graphics;

class DamageText
{
public:
	DamageText();

	void setDamage(Units::HP damage);
	void update(Units::MS dt);
	void draw(Graphics &graphics, Units::Game centerX, Units::Game centerY);
private:
	Units::HP damage = 0;
	Units::Game offsetY = 0.0;
	Timer timer;
};

