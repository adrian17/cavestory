#pragma once

#include "physics/sideType.h"
#include "util/units.h"

class Rectangle
{
public:
	Rectangle(Units::Game x, Units::Game y, Units::Game w, Units::Game h) :
		x(x), y(y), w(w), h(h) {}

	Units::Game centerX() const { return x + w / 2; }
	Units::Game centerY() const { return y + h / 2; }

	Units::Game side(sides::SideType side) const{
		if (side == sides::LEFT_SIDE)
			return left();
		if (side == sides::RIGHT_SIDE)
			return right();
		if (side == sides::TOP_SIDE)
			return top();
		return bottom();
	}

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

