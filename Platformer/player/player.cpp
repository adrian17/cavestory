#include "player.h"

#include "map.h"
#include "particle\headBumpParticle.h"
#include "particle\particleSystem.h"
#include "sprite/animatedSprite.h"
#include "sprite/numberSprite.h"
#include "util\accelerators.h"
#include "util\collisionRectangle.h"
#include "util/rectangle.h"
#include <algorithm>
#include <cmath>

namespace {
	const Units::Acceleration walkingAcceleration = 0.00083007812;
	const Units::Velocity maxVelX = 0.15859375;
	const BidirectionalAccelerators walkingAccelerators(walkingAcceleration, maxVelX);

	const Units::Acceleration friction = 0.00049804687; // (pixels / ms) / ms
	const FrictionAccelerator frictionAccelerator(friction);

	const Units::Velocity jumpSpeed = 0.25;	//jump
	const Units::Velocity shortJumpSpeed = jumpSpeed / 1.5;
	const Units::Acceleration airAcceleration = 0.0003125;
	const Units::Acceleration jumpGravity = 0.0003125;
	const BidirectionalAccelerators airAccelerators(airAcceleration, maxVelX);
	const ConstantAccelerator jumpGravityAccelerator(jumpGravity, terminalSpeed);

	const std::string spriteFilePath = "myChar.bmp";

	const Units::Frame characterFrame = 0;

	const Units::Frame walkFrame = 0;	//motion
	const Units::Frame standFrame = 0;
	const Units::Frame jumpFrame = 1;
	const Units::Frame fallFrame = 2;
	
	const Units::Frame upFrameOffset = 3;	//vertical
	const Units::Frame downFrame = 6;
	const Units::Frame backFrame = 7;

	const Units::Game collisionYTop = 2;
	const Units::Game collisionYHeight = 30;

	const Units::Game collisionYTopWidth = 18;
	const Units::Game collisionYBottomWidth = 10;
	const Units::Game collisionYTopLeft = (Units::tileToGame(1) - collisionYTopWidth) / 2;
	const Units::Game collisionYBottomLeft = (Units::tileToGame(1) - collisionYBottomWidth) / 2;

	const CollisionRectangle collisionRectangle(
		Rectangle(collisionYTopLeft, collisionYTop, collisionYTopWidth, collisionYHeight / 2),
		Rectangle(collisionYBottomLeft, collisionYTop + collisionYHeight / 2,
					collisionYBottomWidth, collisionYHeight / 2),
		Rectangle(6.0, 10.0, 10.0, 12.0),
		Rectangle(16.0, 10.0, 10.0, 12.0));

	const Units::MS invincibleTimeLimit = 3000;
	const Units::MS invincibleFlashTime = 50;
}

Player::Player(Graphics &graphics, ParticleTools &particleTools, Units::Game x, Units::Game y) :
	particleTools(particleTools),
	kinematicsX(x, 0.0),
	kinematicsY(y, 0.0),
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

void Player::update(Units::MS dt, const Map &map){
	sprites[getSpriteState()]->update();

	health.update(dt);
	walkingAnimation.update();
	polarStar.updateProjectiles(dt, map, particleTools);

	updateX(dt, map);
	updateY(dt, map);
}

void Player::updateX(Units::MS dt, const Map &map){
	const Accelerator* accelerator;
	if (onGround){
		if (accX == 0) accelerator = &frictionAccelerator;
		else accelerator = (accX < 0) ? &walkingAccelerators.negative : &walkingAccelerators.positive;
	} else {
		if (accX == 0) accelerator = &ZeroAccelerator::zero;
		else accelerator = (accX < 0) ? &airAccelerators.negative : &airAccelerators.positive;
	}

	MapCollidable::updateX(collisionRectangle, *accelerator, kinematicsX, kinematicsY, dt, map);
}

void Player::updateY(Units::MS dt, const Map &map){
	const Accelerator &accelerator = (jumping && kinematicsY.velocity < 0.0) ? jumpGravityAccelerator : ConstantAccelerator::gravity;

	MapCollidable::updateY(collisionRectangle, accelerator, kinematicsX, kinematicsY, dt, map);
}

void Player::draw(Graphics &graphics){
	if (spriteIsVisible()){
		polarStar.draw(graphics, horizontalFacing, verticalFacing(), gunUp(), kinematicsX.position, kinematicsY.position);
		sprites[getSpriteState()]->draw(graphics, kinematicsX.position, kinematicsY.position);
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
		kinematicsY.velocity = -jumpSpeed;
	} else if (kinematicsY.velocity < 0.0) {	//mid jump
	}
}

void Player::stopJump(){
	jumping = false;
}

void Player::startFire(){
	polarStar.startFire(kinematicsX.position, kinematicsY.position, horizontalFacing, verticalFacing(), gunUp(), particleTools);
}

void Player::stopFire(){
	polarStar.stopFire();
}

void Player::takeDamage(Units::HP damage){
	if (invincibleTimer.active()) return;

	health.takeDamage(damage);
	damageText->setDamage(damage);

	interacting = false;
	kinematicsY.velocity = std::min(-shortJumpSpeed, kinematicsY.velocity);
	invincibleTimer.reset();
}

Rectangle Player::damageRectangle() const{
	return Rectangle(kinematicsX.position + collisionRectangle.boundingBox().left(),
		kinematicsY.position + collisionRectangle.boundingBox().top(),
		collisionRectangle.boundingBox().width(),
		collisionRectangle.boundingBox().height());
}

void Player::onCollision(MapCollidable::SideType side, bool isDeltaDirection){
	switch (side){
	case MapCollidable::TOP_SIDE:
		particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(
			new HeadBumpParticle(particleTools.graphics, centerX(), kinematicsY.position + collisionRectangle.boundingBox().top())));
		if (isDeltaDirection)
			kinematicsY.velocity = 0.0;
		break;
	case MapCollidable::BOTTOM_SIDE:
		onGround = true;
		if (isDeltaDirection)
			kinematicsY.velocity = 0.0;
		break;
	case MapCollidable::LEFT_SIDE:
		if (isDeltaDirection)
			kinematicsX.velocity = 0.0;
		break;
	case MapCollidable::RIGHT_SIDE:
		if (isDeltaDirection)
			kinematicsX.velocity = 0.0;
		break;
	}
}

void Player::onDelta(MapCollidable::SideType side){
	switch (side){
	case MapCollidable::TOP_SIDE:
		onGround = false;
		break;
	case MapCollidable::BOTTOM_SIDE:
		onGround = false;
		break;
	case MapCollidable::LEFT_SIDE:
		break;
	case MapCollidable::RIGHT_SIDE:
		break;
	}
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
	else motion = kinematicsY.velocity < 0.0 ? JUMPING : FALLING;
	return motion;
}

Player::SpriteState Player::getSpriteState(){
	return SpriteState(
		motionType(),
		horizontalFacing,
		verticalFacing(),
		walkingAnimation.stride());
}

bool Player::spriteIsVisible() const{
	return !(invincibleTimer.active() && invincibleTimer.getCurrentTime() / invincibleFlashTime % 3 == 0);
}