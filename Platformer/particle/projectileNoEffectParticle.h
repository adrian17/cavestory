#pragma once

#include "particle\immobileSingleLoopParticle.h"

class ProjectileNoEffectParticle : public ImmobileSingleLoopParticle
{
public:
	ProjectileNoEffectParticle(Graphics &graphics, Units::Game x, Units::Game y);
};

