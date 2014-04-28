#pragma once

#include "util/rectangle.h"
#include "util/units.h"

class Projectile
{
public:
	virtual Rectangle collisionRectangle() const = 0;
	virtual Units::HP contactDamage() const = 0;
	virtual void collideWithEnemy() = 0;
	virtual ~Projectile() = 0;
};

inline Projectile::~Projectile(){}