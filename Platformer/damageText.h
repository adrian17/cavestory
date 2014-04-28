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
	void setPosition(Units::Game centerX, Units::Game centerY);
	void draw(Graphics &graphics);
private:
	Units::HP damage = 0;
	Units::Game centerX = 0, centerY = 0;
	Units::Game offsetY = 0.0;
	Timer timer;
	bool rises = true;
};

