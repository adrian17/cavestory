#include "player.h"

#include "game.h"
#include "sprite/animatedSprite.h"
#include <algorithm>
#include <cmath>

namespace {
	const double slowdownFactor = 0.8;	//walk
	const double walkingAcceleration = 0.0012;
	const double maxVelX = 0.325;

	const double gravity = 0.0012;	//fall
	const double maxVelY = 0.325;

	const double jumpSpeed = 0.325;	//jump
	const int jumpTime = 275;

	const std::string spriteFilePath = "content/myChar.bmp";

	const int characterFrame = 0;

	const int walkFrame = 0;	//motion
	const int standFrame = 0;
	const int jumpFrame = 1;
	const int fallFrame = 2;
	
	const int upFrameOffset = 3;	//vertical
	const int downFrame = 6;
	const int backFrame = 7;

	const int walkFps = 15;	//animation
	const int nWalkFrames = 3;
}

bool operator<(const Player::SpriteState &a, const Player::SpriteState &b){
	if (a.motionType != b.motionType)
		return a.motionType < b.motionType;
	if (a.horizontalFacing != b.horizontalFacing)
		return a.horizontalFacing < b.horizontalFacing;
	if (a.verticalFacing != b.verticalFacing)
		return a.verticalFacing < b.verticalFacing;
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

void Player::loopUp(){
	verticalFacing = UP;
}

void Player::lookDown(){
	verticalFacing = DOWN;
}

void Player::lookHorizontal(){
	verticalFacing = HORIZONTAL;
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
	for (int motionType = 0; motionType < LAST_MOTION_TYPE; ++motionType){
		for (int horizontalFacing = 0; horizontalFacing < LAST_HORIZONTAL_FACING; ++horizontalFacing){
			for (int verticalFacing = 0; verticalFacing < LAST_VERTICAL_FACING; ++verticalFacing){
				initSprite(graphics, SpriteState((MotionType)motionType, (HorizontalFacing)horizontalFacing, (VerticalFacing)verticalFacing));
			}
		}
	}
}

void Player::initSprite(Graphics &graphics, const SpriteState spriteState){
	int srcY = spriteState.horizontalFacing == LEFT ? 
		characterFrame * tileSize : 
		(characterFrame + 1) * tileSize;
	int srcX;
	switch (spriteState.motionType){
	case WALKING:
		srcX = walkFrame * tileSize;
		break;
	case STANDING:
		srcX = standFrame * tileSize;
		break;
	case JUMPING:
		srcX = jumpFrame * tileSize;
		break;
	case FALLING:
		srcX = fallFrame * tileSize;
		break;
	}
	srcX = spriteState.verticalFacing == UP ? srcX + upFrameOffset * tileSize : srcX;

	if (spriteState.motionType == WALKING){
		sprites[spriteState] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, spriteFilePath,
			srcX, srcY, tileSize, tileSize, walkFps, nWalkFrames));
	} else {
		if (spriteState.verticalFacing == DOWN){
			srcX = spriteState.motionType == STANDING ? backFrame * tileSize : downFrame * tileSize;
		}
		sprites[spriteState] = std::unique_ptr<Sprite>(new Sprite(graphics, spriteFilePath,
			srcX, srcY, tileSize, tileSize));
	}
}

Player::SpriteState Player::getSpriteState(){
	MotionType motion;
	if (onGround) motion = accX == 0.0 ? STANDING : WALKING;
	else motion = velY < 0.0 ? JUMPING : FALLING;

	return SpriteState(
		motion,
		horizontalFacing,
		verticalFacing);
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