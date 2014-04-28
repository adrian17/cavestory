#include "particleSystem.h"

#include "particle.h"

void ParticleSystem::update(Units::MS dt){
	for (auto iter = particles.begin(); iter != particles.end();){
		if ((*iter)->update(dt)){
			++iter;
		} else {
			particles.erase(iter++);
		}
	}
}

void ParticleSystem::draw(Graphics &graphics){
	for (auto&& particle : particles){
		particle->draw(graphics);
	}
}