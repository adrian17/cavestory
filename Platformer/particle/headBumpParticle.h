#pragma once

#include "particle\particle.h"
#include "sprite\sprite.h"
#include "util\polarVector.h"
#include "util\timer.h"

class Graphics;

class HeadBumpParticle : public Particle
{
public:
	HeadBumpParticle(Graphics &graphics, Units::Game centerX, Units::Game centerY);

	bool update(Units::MS dt);
	void draw(Graphics &graphics);
private:
	const Units::Game centerX, centerY;
	Sprite sprite;
	Timer timer;
	PolarVector particleA, particleB;
	const Units::Game maxOffsetA, maxOffsetB;
};

