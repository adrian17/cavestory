#pragma once

#include "particle\particle.h"
#include "sprite\animatedSprite.h"
#include "util\polarVector.h"

class Graphics;
class ParticleTools;

class DeathCloudParticle : public Particle
{
public:
	DeathCloudParticle(Graphics &graphics,
		Units::Game centerX, Units::Game centerY,
		Units::Velocity velocity, Units::Degrees angle);

	~DeathCloudParticle();

	bool update(const Units::MS dt);
	void draw(Graphics &graphics) const;

	static void createRandomDeathClouds(ParticleTools particleTools, Units::Game centerX, Units::Game centerY, int nParticles);

private:
	const Units::Game centerX, centerY;
	const Units::Velocity velocity;
	PolarVector offset;
	AnimatedSprite sprite;
};

