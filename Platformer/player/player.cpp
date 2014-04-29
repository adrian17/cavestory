#include "player.h"

#include "map.h"
#include "particle\headBumpParticle.h"
#include "particle\particleSystem.h"
#include "sprite/animatedSprite.h"
#include "sprite/numberSprite.h"
#include "util/rectangle.h"
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

	const std::string spriteFilePath = "myChar.bmp";

	const Units::Frame characterFrame = 0;

	const Units::Frame walkFrame = 0;	//motion
	const Units::Frame standFrame = 0;
	const Units::Frame jumpFrame = 1;
	const Units::Frame fallFrame = 2;
	
	const Units::Frame upFrameOffset = 3;	//vertical
	const Units::Frame downFrame = 6;
	const Units::Frame backFrame = 7;

	const Rectangle collisionX(6.0, 10.0, 20.0, 12.0);

	const Units::Game collisionYTop = 2;
	const Units::Game collisionYHeight = 30;
	const Units::Game collisionYBottom = collisionYTop + collisionYHeight;

	const Units::Game collisionYTopWidth = 18;
	const Units::Game collisionYBottomWidth = 10;
	const Units::Game collisionYTopLeft = (Units::tileToGame(1) - collisionYTopWidth) / 2;
	const Units::Game collisionYBottomLeft = (Units::tileToGame(1) - collisionYBottomWidth) / 2;

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

Player::Player(Graphics &graphics, Units::Game x, Units::Game y) :
	x(x), y(y),
	health(graphics),
	invincibleTimer(invincibleTimeLimit),
	damageText(new DamageText()),
	gunExperienceHUD(graphics),
	polarStar(graphics)
{
	initSprites(graphics);
}

Player::~Player(){
}

void Player::update(Units::MS dt, const Map &map, ParticleTools &particleTools){
	sprites[getSpriteState()]->update();

	health.update(dt);
	walkingAnimation.update();
	polarStar.updateProjectiles(dt, map, particleTools);

	updateX(dt, map, particleTools);
	updateY(dt, map, particleTools);
}

void Player::updateX(Units::MS dt, const Map &map, ParticleTools &particleTools){
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

void Player::updateY(Units::MS dt, const Map &map, ParticleTools &particleTools){
	const Units::Acceleration grav = jumping && velY < 0.0f ? jumpGravity : gravity;
	velY += grav * dt;
	velY = std::min(velY, maxVelY);

	const Units::Game delta = velY * dt;

	if (delta > 0){
		CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
		if (info.collided){
			y = Units::tileToGame(info.row) - collisionYBottom;
			velY = 0.0;
			onGround = true;
		} else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, topCollision(0));
		if (info.collided){
			y = Units::tileToGame(info.row) + collisionYHeight;
			particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(
				new HeadBumpParticle(particleTools.graphics, centerX(), y + collisionYTop)));
		}
	} else {
		CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
		if (info.collided){
			y = Units::tileToGame(info.row) + collisionYHeight;
			particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(
				new HeadBumpParticle(particleTools.graphics, centerX(), y + collisionYTop)));
			velY = 0.0;
		}
		else {
			y += delta;
			onGround = false;
		}
		//other dir
		info = getWallCollisionInfo(map, bottomCollision(0));
		if (info.collided){
			y = Units::tileToGame(info.row) - collisionYBottom;
			onGround = true;
		}
	}
}

void Player::draw(Graphics &graphics){
	if (spriteIsVisible()){
		polarStar.draw(graphics, horizontalFacing, verticalFacing(), gunUp(), x, y);
		sprites[getSpriteState()]->draw(graphics, x, y);
	}
}

void Player::drawHUD(Graphics &graphics){
	if (spriteIsVisible()){
		health.draw(graphics);
		polarStar.drawHUD(graphics, gunExperienceHUD);
	}
}

void Player::startMovingLeft(){
	if (onGround && accX == 0) walkingAnimation.reset();
	accX = -1;
	horizontalFacing = LEFT;
	interacting = false;
}

void Player::startMovingRight(){
	if (onGround && accX == 0) walkingAnimation.reset();
	accX = 1;
	horizontalFacing = RIGHT;
	interacting = false;
}

void Player::stopMoving(){
	accX = 0;
}

void Player::loopUp(){
	intendedVerticalFacing = UP;
	interacting = false;
}

void Player::lookDown(){
	if (intendedVerticalFacing == DOWN) return;
	intendedVerticalFacing = DOWN;
	interacting = (onGround && accX == 0) ? true : false;
}

void Player::lookHorizontal(){
	intendedVerticalFacing = HORIZONTAL;
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

void Player::startFire(ParticleTools &particleTools){
	polarStar.startFire(x, y, horizontalFacing, verticalFacing(), gunUp(), particleTools);
}

void Player::stopFire(){
	polarStar.stopFire();
}

void Player::takeDamage(Units::HP damage){
	if (invincibleTimer.active()) return;

	health.takeDamage(damage);
	damageText->setDamage(damage);

	interacting = false;
	velY = std::min(-shortJumpSpeed, velY);
	invincibleTimer.reset();
}

Rectangle Player::damageRectangle() const{
	return Rectangle(x + collisionX.left(), y + collisionYTop,
		collisionX.width(), collisionYHeight);
}

void Player::initSprites(Graphics &graphics){
	for (int motionType = 0; motionType < LAST_MOTION_TYPE; ++motionType){
		for (int horizontalFacing = 0; horizontalFacing < LAST_HORIZONTAL_FACING; ++horizontalFacing){
			for (int verticalFacing = 0; verticalFacing < LAST_VERTICAL_FACING; ++verticalFacing){
				for (int strideType = 0; strideType < LAST_STRIDE_TYPE; ++strideType){
					initSprite(graphics, SpriteState((MotionType)motionType, (HorizontalFacing)horizontalFacing, 
						(VerticalFacing)verticalFacing, (StrideType)strideType));
				}
			}
		}
	}
}

void Player::initSprite(Graphics &graphics, const SpriteState spriteState){
	MotionType spriteMotionType = std::get<0>(spriteState);
	HorizontalFacing spriteHorizontalFacing = std::get<1>(spriteState);
	VerticalFacing spriteVerticalFacing = std::get<2>(spriteState);
	StrideType strideType = std::get<3>(spriteState);

	Units::Tile tileY = spriteHorizontalFacing == LEFT ? characterFrame : characterFrame + 1;
	Units::Tile tileX;

	if (spriteMotionType == WALKING) tileX = walkFrame;
	else if (spriteMotionType == STANDING) tileX = standFrame;
	else if (spriteMotionType == INTERACTING) tileX = backFrame;
	else if (spriteMotionType == JUMPING) tileX = jumpFrame;
	else if (spriteMotionType == FALLING) tileX = fallFrame;

	if (spriteVerticalFacing == UP) tileX += upFrameOffset;
	else if (spriteVerticalFacing == DOWN) tileX = downFrame;

	if (spriteMotionType == WALKING){
		if (strideType == STRIDE_LEFT) tileX += 1;
		else if (strideType == STRIDE_RIGHT) tileX += 2;
		sprites[spriteState].reset(new Sprite(graphics, spriteFilePath,
			Units::tileToPixel(tileX), Units::tileToPixel(tileY),
			Units::tileToPixel(1), Units::tileToPixel(1)));
	} else {
		sprites[spriteState].reset(new Sprite(graphics, spriteFilePath,
			Units::tileToPixel(tileX), Units::tileToPixel(tileY),
			Units::tileToPixel(1), Units::tileToPixel(1)));
	}
}

Player::MotionType Player::motionType() const{
	MotionType motion;

	if (interacting) motion = INTERACTING;
	else if (onGround) motion = accX == 0 ? STANDING : WALKING;
	else motion = velY < 0.0 ? JUMPING : FALLING;
	return motion;
}

Player::SpriteState Player::getSpriteState(){


	return SpriteState(
		motionType(),
		horizontalFacing,
		verticalFacing(),
		walkingAnimation.stride());
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
		x + collisionYTopLeft,
		y + collisionYTop + delta,
		collisionYTopWidth,
		collisionYHeight / 2 - delta);
}

Rectangle Player::bottomCollision(Units::Game delta) const{
	_ASSERT(delta >= 0);
	return Rectangle(
		x + collisionYBottomLeft,
		y + collisionYTop + collisionYHeight / 2,
		collisionYBottomWidth,
		collisionYHeight / 2 + delta);
}

bool Player::spriteIsVisible() const{
	return !(invincibleTimer.active() && invincibleTimer.getCurrentTime() / invincibleFlashTime % 3 == 0);
}