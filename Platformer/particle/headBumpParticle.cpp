#include "headBumpParticle.h"

#include <algorithm>
#include <cstdlib>

namespace {
	const Units::Pixel sourceX = Units::gameToPixel(116);
	const Units::Pixel sourceY = Units::gameToPixel(54);

	const Units::Pixel width = Units::gameToPixel(6);
	const Units::Pixel height = Units::gameToPixel(6);

	const Units::MS lifetime = 600;
	const Units::MS flashPeroid = 25;

	const Units::Velocity velocity = 0.12;
}

HeadBumpParticle::HeadBumpParticle(Graphics &graphics, Units::Game centerX, Units::Game centerY) :
	centerX(centerX), centerY(centerY),
	sprite(graphics, "Caret.bmp", sourceX, sourceY, width, height),
	timer(lifetime, true),
	particleA(0, (Units::Degrees)(rand() % 360)),
	maxOffsetA(4 + rand() % 16),
	particleB(0, (Units::Degrees)(rand() % 360)),
	maxOffsetB(4 + rand() % 16)
{
}

bool HeadBumpParticle::update(Units::MS dt){
	particleA.magnitude = std::min(particleA.magnitude + velocity * dt, maxOffsetA);
	particleB.magnitude = std::min(particleB.magnitude + velocity * dt, maxOffsetB);
	return timer.active();
}

void HeadBumpParticle::draw(Graphics &graphics){
	if (timer.getCurrentTime() / flashPeroid % 3 == 0){
		sprite.draw(graphics, centerX + particleA.getX(), centerY + particleA.getY());
		sprite.draw(graphics, centerX + particleB.getX(), centerY + particleB.getY());
	}
}