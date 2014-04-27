#include "damageText.h"

#include "graphics.h"
#include "sprite\numberSprite.h"
#include <algorithm>

namespace {
	const Units::Velocity velocity = -Units::halfTile / 250;
	const Units::MS damageTime = 2000;
}

DamageText::DamageText() :
	timer(damageTime)
{
}

void DamageText::setDamage(Units::HP newDamage){
	damage = newDamage;
	offsetY = 0.0;
	timer.reset();
}

void DamageText::update(Units::MS dt){
	if (timer.expired()) return;
	offsetY = std::max(-Units::tileToGame(1), offsetY + velocity * dt);
}

void DamageText::draw(Graphics &graphics, Units::Game centerX, Units::Game centerY){
	if (timer.expired()) return;
	NumberSprite::DamageNumber(graphics, damage).drawCentered(graphics, centerX, centerY+offsetY);
}