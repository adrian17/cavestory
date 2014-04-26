#include "player.h"

#include "game.h"
#include "map.h"
#include "rectangle.h"
#include "sprite/animatedSprite.h"
#include <algorithm>
#include <cmath>

namespace {
	const double slowdownFactor = 0.8;	//walk
	const double walkingAcceleration = 0.0012;
	const double maxVelX = 0.325;

	const double gravity = 0.0012;	//fall
	const double maxVelY = 0.325;

	const double jumpSpeed = 0.25;	//jump
	const double airAcceleration = 0.0003125;
	const double jumpGravity = 0.0003125;

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

	const Rectangle collisionX(6, 10, 20, 12);
	const Rectangle collisionY(10, 2, 12, 30);

	struct CollisionInfo{
		bool collided;
		int row, col;
	} info;

	CollisionInfo getWallCollisionInfo(const Map &map, const Rectangle rectangle){
		CollisionInfo info = { false, 0, 0 };
		std::vector<Map::CollisionTile> tiles = map.getCollidingTiles(rectangle);
		for (auto && tile : tiles){
			if (tile.tileType == Map::WALL_TILE){
				info = { true, tile.row, tile.col };
				break;
			}
		}
		return info;
	}
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

void Player::update(int dt, const Map &map){
	sprites[getSpriteState()]->update(dt);

	updateX(dt, map);
	updateY(dt, map);
}

void Player::updateX(int dt, const Map &map){
	velX += accX * dt;

	if (accX<0.0) velX = std::max(velX, -maxVelX);
	else if (accX>0.0) velX = std::min(velX, maxVelX);
	else if (onGround) velX *= slowdownFactor;

	const int delta = (int)round(velX * dt);
	if (delta > 0){
		CollisionInfo info = getWallCollisionInfo(map, rightCollision(delta));
		if (info.collided){
			x = info.col * tileSize - collisionX.right();
			velX = 0.0;
		}
		else
			x += delta;
		//other dir
		info = getWallCollisionInfo(map, leftCollision(0));
		if (info.collided){
			x = info.col * tileSize + collisionX.right();
		}
	}
	else {
		CollisionInfo info = getWallCollisionInfo(map, leftCollision(delta));
		if (info.collided){
			x = info.col * tileSize + collisionX.right();
			velX = 0.0;
		}
		else
			x += delta;
		//other dir
		info = getWallCollisionInfo(map, rightCollision(0));
		if (info.collided){
			x = info.col * tileSize - collisionX.right();
		}
	}
}

void Player::updateY(int dt, const Map &map){
	const double grav = jumping && velY < 0.0f ? jumpGravity : gravity;
	velY += grav * dt;
	velY = std::min(velY, maxVelY);

	const int delta = (int)round(velY * dt);

	if (delta > 0){
		CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
		if (info.collided){
			y = info.row * tileSize - collisionY.bottom();
			velY = 0.0;
			onGround = true;
		} else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, topCollision(0));
		if (info.collided){
			y = info.row * tileSize + collisionY.height();
		}
	} else {
		CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
		if (info.collided){
			y = info.row * tileSize + collisionY.height();
			velY = 0.0;
		}
		else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, bottomCollision(0));
		if (info.collided){
			y = info.row * tileSize - collisionY.bottom();
			onGround = true;
		}
	}
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
	jumping = true;
	if (onGround){
		velY = -jumpSpeed;
	} else if (velY < 0.0) {	//mid jump
	}
}

void Player::stopJump(){
	jumping = false;
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

Rectangle Player::leftCollision(int delta) const{
	_ASSERT(delta <= 0);
	return Rectangle(
		(int)round(x) + collisionX.left() + delta,
		(int)round(y) + collisionX.top(),
		collisionX.width() / 2 - delta,
		collisionX.height());
}

Rectangle Player::rightCollision(int delta) const{
	_ASSERT(delta >= 0);
	return Rectangle(
		(int)round(x) + collisionX.left() + collisionX.width() / 2,
		(int)round(y) + collisionX.top(),
		collisionX.width() / 2 + delta,
		collisionX.height());
}

Rectangle Player::topCollision(int delta) const{
	_ASSERT(delta <= 0);
	return Rectangle(
		(int)round(x) + collisionY.left(),
		(int)round(y) + collisionY.top() + delta,
		collisionY.width(),
		collisionY.height() / 2 - delta);
}

Rectangle Player::bottomCollision(int delta) const{
	_ASSERT(delta >= 0);
	return Rectangle(
		(int)round(x) + collisionY.left(),
		(int)round(y) + collisionY.top() + collisionY.height() / 2,
		collisionY.width(),
		collisionY.height() / 2 + delta);
}