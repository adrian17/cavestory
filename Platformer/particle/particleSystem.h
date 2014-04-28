#pragma once

#include "util\units.h"
#include <memory>
#include <set>

class Graphics;
class Particle;

class ParticleSystem
{
public:
	void addNewParticle(std::shared_ptr<Particle> particle)
		{ particles.insert(particle); }

	void update(Units::MS dt);
	void draw(Graphics &graphics);
private:
	typedef std::set<std::shared_ptr<Particle>> ParticleSet;
	ParticleSet particles;
};

class ParticleTools{
public:
	ParticleSystem &system;
	Graphics &graphics;
};