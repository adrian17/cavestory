#pragma once

#include "util\rectangle.h"

class Graphics;
class Map;

class Pickup{
public:
	enum PickupType { HEALTH, MISSLE, EXPERIENCE };

	virtual Rectangle collisionRectangle() const = 0;

	virtual bool update(const Units::MS dt, const Map &map) = 0;
	virtual void draw(Graphics &graphics) const = 0;
	virtual int value() const = 0;
	virtual PickupType type() const = 0;

	virtual ~Pickup() = 0;
};
inline Pickup::~Pickup(){}