#include "player.h"

#include "map.h"
#include "util/rectangle.h"
#include "sprite/animatedSprite.h"
#include "sprite/numberSprite.h"
#include <algorithm>
#include <cmath>

namespace {
	const Units::Acceleration walkingAcceleration = 0.00083007812;
	const Units::Velocity maxVelX = 0.15859375;
	const Units::Acceleration friction = 0.00049804687; // (pixels / ms) / ms

	const Units::Acceleration gravity = 0.00078125;	//fall
	const Units::Velocity maxVelY = 0.2998046875;

	const Units::Velocity jumpSpeed = 0.25;	//jump
	const Units::Velocity shortJumpSpeed = jumpSpeed / 1.5;
	const Units::Acceleration airAcceleration = 0.0003125;
	const Units::Acceleration jumpGravity = 0.0003125;

	const std::string spriteFilePath = "content/myChar.bmp";

	const Units::Frame characterFrame = 0;

	const Units::Frame walkFrame = 0;	//motion
	const Units::Frame standFrame = 0;
	const Units::Frame jumpFrame = 1;
	const Units::Frame fallFrame = 2;
	
	const Units::Frame upFrameOffset = 3;	//vertical
	const Units::Frame downFrame = 6;
	const Units::Frame backFrame = 7;

	const Units::MS walkFps = 15;	//animation
	const Units::Frame nWalkFrames = 3;

	const Rectangle collisionX(6.0, 10.0, 20.0, 12.0);
	const Rectangle collisionY(10.0, 2.0, 12.0, 30.0);

	const Units::MS invincibleTimeLimit = 3000;
	const Units::MS invincibleFlashTime = 50;

	struct CollisionInfo{
		bool collided;
		Units::Tile row, col;
	} info;

	CollisionInfo getWallCollisionInfo(const Map &map, const Rectangle &rectangle){
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

Player::Player(Graphics &graphics, Units::Game x, Units::Game y) :
	x(x), y(y),
	health(graphics),
	invincibleTimer(invincibleTimeLimit),
	polarStar(graphics)
{
	initSprites(graphics);
}

Player::~Player(){
}

void Player::update(Units::MS dt, const Map &map){
	sprites[getSpriteState()]->update(dt);

	health.update(dt);
	damageText.update(dt);

	updateX(dt, map);
	updateY(dt, map);
}

void Player::updateX(Units::MS dt, const Map &map){
	Units::Acceleration acceleration = 0.0f;
	if (accX < 0) acceleration = onGround ? -walkingAcceleration : -airAcceleration;
	if (accX > 0) acceleration = onGround ? walkingAcceleration : airAcceleration;
	velX += acceleration * dt;

	if (accX<0) velX = std::max(velX, -maxVelX);
	else if (accX>0) velX = std::min(velX, maxVelX);
	else if (onGround) velX = velX > 0.f ? std::max(0.0, velX - friction*dt) : std::min(0.0, velX + friction*dt);

	const Units::Game delta = velX * dt;

	if (delta > 0.0){
		CollisionInfo info = getWallCollisionInfo(map, rightCollision(delta));
		if (info.collided){
			x = Units::tileToGame(info.col) - collisionX.right();
			velX = 0.0;
		}
		else
			x += delta;
		//other dir
		info = getWallCollisionInfo(map, leftCollision(0));
		if (info.collided){
			x = Units::tileToGame(info.col) + collisionX.right();
		}
	}
	else {
		CollisionInfo info = getWallCollisionInfo(map, leftCollision(delta));
		if (info.collided){
			x = Units::tileToGame(info.col) + collisionX.right();
			velX = 0.0;
		}
		else
			x += delta;
		//other dir
		info = getWallCollisionInfo(map, rightCollision(0));
		if (info.collided){
			x = Units::tileToGame(info.col) - collisionX.right();
		}
	}
}

void Player::updateY(Units::MS dt, const Map &map){
	const Units::Acceleration grav = jumping && velY < 0.0f ? jumpGravity : gravity;
	velY += grav * dt;
	velY = std::min(velY, maxVelY);

	const Units::Game delta = velY * dt;

	if (delta > 0){
		CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
		if (info.collided){
			y = Units::tileToGame(info.row) - collisionY.bottom();
			velY = 0.0;
			onGround = true;
		} else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, topCollision(0));
		if (info.collided){
			y = Units::tileToGame(info.row) + collisionY.height();
		}
	} else {
		CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
		if (info.collided){
			y = Units::tileToGame(info.row) + collisionY.height();
			velY = 0.0;
		}
		else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, bottomCollision(0));
		if (info.collided){
			y = Units::tileToGame(info.row) - collisionY.bottom();
			onGround = true;
		}
	}
}

void Player::draw(Graphics &graphics){
	if (spriteIsVisible()){
		polarStar.draw(graphics, horizontalFacing, verticalFacing, x, y);
		sprites[getSpriteState()]->draw(graphics, x, y);
	}
}

void Player::drawHUD(Graphics &graphics){
	if (spriteIsVisible()){
		health.draw(graphics);
	}
	damageText.draw(graphics, centerX(), centerY());
}

void Player::startMovingLeft(){
	accX = -1;
	horizontalFacing = LEFT;
	interacting = false;
}

void Player::startMovingRight(){
	accX = 1;
	horizontalFacing = RIGHT;
	interacting = false;
}

void Player::stopMoving(){
	accX = 0;
}

void Player::loopUp(){
	verticalFacing = UP;
	interacting = false;
}

void Player::lookDown(){
	if (verticalFacing == DOWN) return;
	verticalFacing = DOWN;
	interacting = (onGround && accX == 0) ? true : false;
}

void Player::lookHorizontal(){
	verticalFacing = HORIZONTAL;
}

void Player::startJump(){
	interacting = false;
	jumping = true;
	if (onGround){
		velY = -jumpSpeed;
	} else if (velY < 0.0) {	//mid jump
	}
}

void Player::stopJump(){
	jumping = false;
}

void Player::takeDamage(Units::HP damage){
	if (invincibleTimer.active()) return;

	health.takeDamage(damage);
	damageText.setDamage(damage);

	interacting = false;
	velY = std::min(-shortJumpSpeed, velY);
	invincibleTimer.reset();
}

Rectangle Player::damageRectangle() const{
	return Rectangle(x + collisionX.left(), y + collisionY.top(),
		collisionX.width(), collisionY.height());
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
	Units::Tile tileY = spriteState.horizontalFacing == LEFT ? characterFrame : characterFrame + 1;
	Units::Tile tileX;
	switch (spriteState.motionType){
	case WALKING:
		tileX = walkFrame;
		break;
	case STANDING:
		tileX = standFrame;
		break;
	case INTERACTING:
		tileX = backFrame;
		break;
	case JUMPING:
		tileX = jumpFrame;
		break;
	case FALLING:
		tileX = fallFrame;
		break;
	}
	tileX = spriteState.verticalFacing == UP ? tileX + upFrameOffset : tileX;

	if (spriteState.motionType == WALKING){
		sprites[spriteState] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, spriteFilePath,
			Units::tileToPixel(tileX), Units::tileToPixel(tileY),
			Units::tileToPixel(1), Units::tileToPixel(1),
			walkFps, nWalkFrames));
	} else {
		if (spriteState.verticalFacing == DOWN && (spriteState.motionType == JUMPING || spriteState.motionType == FALLING)){
			tileX = downFrame;
		}
		sprites[spriteState] = std::unique_ptr<Sprite>(new Sprite(graphics, spriteFilePath,
			Units::tileToPixel(tileX), Units::tileToPixel(tileY),
			Units::tileToPixel(1), Units::tileToPixel(1)));
	}
}

Player::SpriteState Player::getSpriteState(){
	MotionType motion;
	if (interacting) motion = INTERACTING;
	else if (onGround) motion = accX == 0 ? STANDING : WALKING;
	else motion = velY < 0.0 ? JUMPING : FALLING;

	return SpriteState(
		motion,
		horizontalFacing,
		verticalFacing);
}

Rectangle Player::leftCollision(Units::Game delta) const{
	_ASSERT(delta <= 0);
	return Rectangle(
		x + collisionX.left() + delta,
		y + collisionX.top(),
		collisionX.width() / 2 - delta,
		collisionX.height());
}

Rectangle Player::rightCollision(Units::Game delta) const{
	_ASSERT(delta >= 0);
	return Rectangle(
		x + collisionX.left() + collisionX.width() / 2,
		y + collisionX.top(),
		collisionX.width() / 2 + delta,
		collisionX.height());
}

Rectangle Player::topCollision(Units::Game delta) const{
	_ASSERT(delta <= 0);
	return Rectangle(
		x + collisionY.left(),
		y + collisionY.top() + delta,
		collisionY.width(),
		collisionY.height() / 2 - delta);
}

Rectangle Player::bottomCollision(Units::Game delta) const{
	_ASSERT(delta >= 0);
	return Rectangle(
		x + collisionY.left(),
		y + collisionY.top() + collisionY.height() / 2,
		collisionY.width(),
		collisionY.height() / 2 + delta);
}

bool Player::spriteIsVisible() const{
	return !(invincibleTimer.active() && invincibleTimer.getCurrentTime() / invincibleFlashTime % 3 == 0);
}