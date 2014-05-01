#pragma once

#include "util\timer.h"
#include "util\units.h"

class Graphics;

class FloatingNumber
{
public:
	enum NumberType { DAMAGE, EXPERIENCE };
	FloatingNumber(NumberType type);

	void addValue(int val);
	bool update(Units::MS dt);
	void setPosition(Units::Game centerX, Units::Game centerY);
	void draw(Graphics &graphics);
private:
	int value = 0;
	Units::Game centerX = 0, centerY = 0;
	Units::Game offsetY = 0.0;
	Timer timer;
	bool rises = true;
	const NumberType type;
};

