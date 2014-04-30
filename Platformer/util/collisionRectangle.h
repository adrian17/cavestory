#pragma once

#include "util\rectangle.h"
#include <crtdbg.h>

class CollisionRectangle{
public:

	CollisionRectangle(const Rectangle &top, Rectangle &bottom, Rectangle &left, Rectangle &right) :
		top(top), bottom(bottom), left(left), right(right)
	{}

	Rectangle boundingBox() const {
		return Rectangle(
			left.left(), top.top(),
			left.width() + right.width(),
			top.height() + bottom.height());
	}

	Rectangle leftCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta <= 0);
		return Rectangle(
			x + left.left() + delta,
			y + left.top(),
			left.width() - delta,
			left.height());
	}
	Rectangle rightCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta >= 0);
		return Rectangle(
			x + right.left(),
			y + right.top(),
			right.width() + delta,
			right.height());
	}
	Rectangle topCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta <= 0);
		return Rectangle(
			x + top.left(),
			y + top.top() + delta,
			top.width(),
			top.height() - delta);
	}
	Rectangle bottomCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta >= 0);
		return Rectangle(
			x + bottom.left(),
			y + bottom.top(),
			bottom.width(),
			bottom.height() + delta);
	}

private:
	Rectangle top, bottom, left, right;
};