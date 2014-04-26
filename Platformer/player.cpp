#include "player.h"

#include "game.h"
#include "animatedSprite.h"
#include <algorithm>
#include <cmath>

namespace {
	const double slowdownFactor = 0.8;
	const double walkingAcceleration = 0.0012;
	const double maxVelX = 0.325;
}

Player::Player(Graphics &graphics, int x, int y):
	x(x), y(y)
{
	sprite.reset(new AnimatedSprite(graphics, "content/myChar.bmp", 0, 0, tileSize, tileSize, 15, 3));
}

Player::~Player(){
}

void Player::update(int dt){
	sprite->update(dt);

	x += velX * dt;
	velX += accX * dt;

	if (accX<0.0) velX = std::max(velX, -maxVelX);
	else if (accX>0.0) velX = std::min(velX, maxVelX);
	else velX *= slowdownFactor;
}

void Player::draw(Graphics &graphics){
	sprite->draw(graphics, (int)round(x), (int)round(y));
}

void Player::startMovingLeft(){
	accX = -walkingAcceleration;
}

void Player::startMovingRight(){
	accX = walkingAcceleration;
}

void Player::stopMoving(){
	accX = 0.0;
}