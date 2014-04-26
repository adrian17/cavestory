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

bool operator<(const Player::SpriteState &a, const Player::SpriteState &b){
	if (a.motionType != b.motionType)
		return a.motionType < b.motionType;
	if (a.horizontalFacing != b.horizontalFacing)
		return a.horizontalFacing < b.horizontalFacing;
	return false;
}

Player::Player(Graphics &graphics, int x, int y):
	x(x), y(y)
{
	initSprites(graphics);
}

Player::~Player(){
}

void Player::update(int dt){
	sprites[getSpriteState()]->update(dt);

	x += velX * dt;
	velX += accX * dt;

	if (accX<0.0) velX = std::max(velX, -maxVelX);
	else if (accX>0.0) velX = std::min(velX, maxVelX);
	else velX *= slowdownFactor;
}

void Player::draw(Graphics &graphics){
	sprites[getSpriteState()]->draw(graphics, (int)round(x), (int)round(y));
}

void Player::startMovingLeft(){
	accX = -walkingAcceleration;
	horizontalFacing = LEFT;
}

void Player::startMovingRight(){
	accX = walkingAcceleration;
	horizontalFacing = RIGHT;
}

void Player::stopMoving(){
	accX = 0.0;
}

Player::SpriteState Player::getSpriteState(){
	return SpriteState(
		accX == 0.0 ? STANDING : WALKING,
		horizontalFacing);
}

void Player::initSprites(Graphics &graphics){
	sprites[SpriteState(STANDING, LEFT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		0, 0,
		tileSize, tileSize));
	sprites[SpriteState(WALKING, LEFT)] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, "content/myChar.bmp",
		0, 0,
		tileSize, tileSize, 15, 3));
	sprites[SpriteState(STANDING, RIGHT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		0, tileSize,
		tileSize, tileSize));
	sprites[SpriteState(WALKING, RIGHT)] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, "content/myChar.bmp",
		0, tileSize,
		tileSize, tileSize, 15, 3));

}