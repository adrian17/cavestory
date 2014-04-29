#pragma once

#include "particle\particle.h"
#include "sprite\animatedSprite.h"
#include "util\polarVector.h"

class Graphics;

class DeathCloudParticle : public Particle
{
public:
	DeathCloudParticle(Graphics &graphics,
		Units::Game centerX, Units::Game centerY,
		Units::Velocity velocity, Units::Degrees angle);

	~DeathCloudParticle();

	bool update(Units::MS dt);
	void draw(Graphics &graphics);

private:
	const Units::Game centerX, centerY;
	const Units::Velocity velocity;
	PolarVector offset;
	AnimatedSprite sprite;
};

