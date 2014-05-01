#pragma once

#include "units.h"

class Rectangle
{
public:
	Rectangle(Units::Game x, Units::Game y, Units::Game w, Units::Game h) :
		x(x), y(y), w(w), h(h) {}

	Units::Game centerX() const { return x + w / 2; }
	Units::Game centerY() const { return y + h / 2; }

	Units::Game left() const{ return x; }
	Units::Game right() const{ return x + w; }
	Units::Game top() const{ return y; }
	Units::Game bottom() const{ return y + h; }
	Units::Game width() const{ return w; }
	Units::Game height() const{ return h; }

	bool collidesWith(const Rectangle &other) const{
		return right() >= other.left() &&
			left() <= other.right() &&
			top() <= other.bottom() &&
			bottom() >= other.top();
	}
private:
	const Units::Game x, y, w, h;
};

