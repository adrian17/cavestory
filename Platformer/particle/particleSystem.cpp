#include "particleSystem.h"

#include "particle.h"

void ParticleSystem::update(const Units::MS dt){
	for (auto iter = particles.begin(); iter != particles.end();){
		if ((*iter)->update(dt)){
			++iter;
		} else {
			particles.erase(iter++);
		}
	}
}

void ParticleSystem::draw(Graphics &graphics) const{
	for (auto&& particle : particles){
		particle->draw(graphics);
	}
}