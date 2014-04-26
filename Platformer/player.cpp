#include "player.h"

#include "game.h"
#include "animatedSprite.h"
#include <algorithm>
#include <cmath>

namespace {
	const double slowdownFactor = 0.8;
	const double walkingAcceleration = 0.0012;
	const double maxVelX = 0.325;

	const double gravity = 0.0012;
	const double maxVelY = 0.325;

	const double jumpSpeed = 0.325;
	const int jumpTime = 275;

	const int jumpFrame = 1;
	const int fallFrame = 2;
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

	jump.update(dt);

	x += velX * dt;
	y += velY * dt;
	velX += accX * dt;

	if (accX<0.0) velX = std::max(velX, -maxVelX);
	else if (accX>0.0) velX = std::min(velX, maxVelX);
	else if (onGround) velX *= slowdownFactor;

	if (jump.isActive() == false){
		velY += gravity * dt;
		velY = std::min(velY, maxVelY);
	}

	if (y > 320){
		y = 320;
		velY = 0.0;
	}
	onGround = (y == 320);
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

void Player::startJump(){
	if (onGround){
		jump.reset();
		velY = -jumpSpeed;
	} else if (velY < 0.0) {	//mid jump
		jump.reactivate();
	}
}

void Player::stopJump(){
	jump.deactivate();
}

void Player::initSprites(Graphics &graphics){
	sprites[SpriteState(STANDING, LEFT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		0, 0,
		tileSize, tileSize));
	sprites[SpriteState(WALKING, LEFT)] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, "content/myChar.bmp",
		0, 0,
		tileSize, tileSize, 15, 3));
	sprites[SpriteState(JUMPING, LEFT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		jumpFrame*tileSize, 0,
		tileSize, tileSize));
	sprites[SpriteState(FALLING, LEFT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		fallFrame*tileSize, 0,
		tileSize, tileSize));
	sprites[SpriteState(STANDING, RIGHT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		0, tileSize,
		tileSize, tileSize));
	sprites[SpriteState(WALKING, RIGHT)] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, "content/myChar.bmp",
		0, tileSize,
		tileSize, tileSize, 15, 3));
	sprites[SpriteState(JUMPING, RIGHT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		jumpFrame*tileSize, tileSize,
		tileSize, tileSize));
	sprites[SpriteState(FALLING, RIGHT)] = std::unique_ptr<Sprite>(new Sprite(graphics, "content/myChar.bmp",
		fallFrame*tileSize, tileSize,
		tileSize, tileSize));

}

Player::SpriteState Player::getSpriteState(){
	MotionType motion;
	if (onGround) motion = accX == 0.0 ? STANDING : WALKING;
	else motion = velY < 0.0 ? JUMPING : FALLING;

	return SpriteState(
		motion,
		horizontalFacing);
}

void Player::Jump::update(int dt){
	if (active){
		timeRemaining -= dt;
		if (timeRemaining <= 0)
			deactivate();
	}
}

void Player::Jump::reset(){
	timeRemaining = jumpTime;
	reactivate();
}