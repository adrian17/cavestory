#pragma once

#include "particle\particle.h"
#include "sprite\animatedSprite.h"

class ProjectileStarParticle : public Particle
{
public:
	ProjectileStarParticle(Graphics &graphics, Units::Game x, Units::Game y);

	bool update(Units::MS dt);
	void draw(Graphics &graphics);
private:
	const Units::Game x, y;
	AnimatedSprite sprite;
};

