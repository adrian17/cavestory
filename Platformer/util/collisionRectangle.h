#pragma once

#include "util\rectangle.h"
#include <crtdbg.h>

class CollisionRectangle{
public:
	virtual Rectangle boundingBox() const = 0;
	virtual Rectangle leftCollision(Units::Game x, Units::Game y, Units::Game delta) const = 0;
	virtual Rectangle rightCollision(Units::Game x, Units::Game y, Units::Game delta) const = 0;
	virtual Rectangle topCollision(Units::Game x, Units::Game y, Units::Game delta) const = 0;
	virtual Rectangle bottomCollision(Units::Game x, Units::Game y, Units::Game delta) const = 0;
	virtual ~CollisionRectangle() = 0;
};
inline CollisionRectangle::~CollisionRectangle(){}

class SimpleCollisionRectangle : public CollisionRectangle{
public:

	SimpleCollisionRectangle(const Rectangle &rectangle) :
		rectangle(rectangle)
	{}

	Rectangle boundingBox() const {
		const Units::Game extraOffset = 0.001;
		return Rectangle(
			rectangle.left() - extraOffset,
			rectangle.top() - extraOffset,
			rectangle.width() + 2 * extraOffset,
			rectangle.height() + 2 * extraOffset);
	}

	Rectangle leftCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta <= 0);
		return Rectangle(
			x + rectangle.left() + delta,
			y + rectangle.top(),
			rectangle.width() - delta,
			rectangle.height());
	}
	Rectangle rightCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta >= 0);
		return Rectangle(
			x + rectangle.left(),
			y + rectangle.top(),
			rectangle.width() + delta,
			rectangle.height());
	}
	Rectangle topCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta <= 0);
		return Rectangle(
			x + rectangle.left(),
			y + rectangle.top() + delta,
			rectangle.width(),
			rectangle.height() - delta);
	}
	Rectangle bottomCollision(Units::Game x, Units::Game y, Units::Game delta) const{
		_ASSERT(delta >= 0);
		return Rectangle(
			x + rectangle.left(),
			y + rectangle.top(),
			rectangle.width(),
			rectangle.height() + delta);
	}

private:
	Rectangle rectangle;
};

class CompositeCollisionRectangle : public CollisionRectangle{
public:

	CompositeCollisionRectangle(const Rectangle &top, Rectangle &bottom, Rectangle &left, Rectangle &right) :
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