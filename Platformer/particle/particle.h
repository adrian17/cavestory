#pragma once

#include "util/units.h"

class Graphics;

class Particle{
public:
	virtual bool update(const Units::MS dt) = 0;
	virtual void draw(Graphics &graphics) const = 0;

	virtual ~Particle() = 0;
};

inline Particle::~Particle(){}