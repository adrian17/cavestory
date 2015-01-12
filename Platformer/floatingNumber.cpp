#include "floatingNumber.h"

#include "sprite/numberSprite.h"
#include <algorithm>

namespace {
	const Units::Velocity velocity = -Units::halfTile / 250;
	const Units::MS damageTime = 2000;
}

FloatingNumber::FloatingNumber(NumberType type) :
	timer(damageTime),
	type(type)
{
}

void FloatingNumber::addValue(int val){
	rises = (value == 0);
	if (rises)
		offsetY = 0.0;
	value += val;
	timer.reset();
}

bool FloatingNumber::update(const Units::MS dt){
	if (timer.expired()) {
		value = 0;
	}
	else if(rises)
		offsetY = std::max(-Units::tileToGame(1), offsetY + velocity * dt);
	return !timer.expired();
}

void FloatingNumber::setPosition(Units::Game centerX, Units::Game centerY){
	this->centerX = centerX;
	this->centerY = centerY;
}

void FloatingNumber::draw(Graphics &graphics) const{
	if (timer.expired()) return;
	if(type == DAMAGE)
		NumberSprite::DamageNumber(graphics, value).drawCentered(graphics, centerX, centerY + offsetY);
	else
		NumberSprite::ExperienceNumber(graphics, value).drawCentered(graphics, centerX, centerY + offsetY);
}