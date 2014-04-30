#pragma once

#include "damageText.h"
#include "mapCollidable.h"
#include "spriteState.h"
#include "interfaces\damageable.h"
#include "player\gunExperienceHUD.h"
#include "sprite\sprite.h"
#include "sprite/varyingWidthSprite.h"
#include "util\kinematics.h"
#include "util\timer.h"
#include "util/units.h"
#include "weapons\polarStar.h"
#include <map>
#include <memory>
#include <tuple>

class Map;
class Graphics;
class ParticleTools;
class Pickup;
class Projectile;
class Rectangle;

class Player : public Damageable, private MapCollidable
{
public:
	Player(Graphics &graphics, ParticleTools &particleTools, Units::Game x, Units::Game y);
	~Player();

	void update(Units::MS dt, const Map &map);
	void updateX(Units::MS dt, const Map &map);
	void updateY(Units::MS dt, const Map &map);
	void draw(Graphics &graphics);
	void drawHUD(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void loopUp();
	void lookDown();
	void lookHorizontal();

	void startJump();
	void stopJump();

	void startFire();
	void stopFire();

	void takeDamage(Units::HP damage);
	void collectPickup(const Pickup &pickup);

	Rectangle damageRectangle() const;

	Units::Game centerX() const{ return kinematicsX.position + Units::halfTile; }
	Units::Game centerY() const{ return kinematicsY.position + Units::halfTile; }
	std::shared_ptr<DamageText> getDamageText() { return damageText; }

	std::vector<std::shared_ptr< ::Projectile>> getProjectiles(){
		return polarStar.getProjectiles();
	}

private:
	enum MotionType { STANDING, INTERACTING, WALKING, JUMPING, FALLING, LAST_MOTION_TYPE };
	enum StrideType { STRIDE_MIDDLE, STRIDE_LEFT, STRIDE_RIGHT, LAST_STRIDE_TYPE };

	typedef std::tuple<MotionType, HorizontalFacing, VerticalFacing, StrideType> SpriteState;
	SpriteState getSpriteState();

	struct WalkingAnimation{
		WalkingAnimation();
		StrideType stride() const;
		void update();
		void reset();
	private:
		Units::Frame currentFrame = 0;
		Timer frameTimer;
		bool forward = true;
	};

	struct Health {
		Health(Graphics &graphics);
		void update(Units::MS dt);
		void draw(Graphics &graphics);
		bool takeDamage(Units::HP damage);	//true if dies
	private:
		Units::HP currentHealth = 6;
		Units::HP maxHealth = 6;
		Sprite healthBarSprite;
		VaryingWidthSprite healthFillSprite;
		VaryingWidthSprite damageFillSprite;

		Units::HP damageTaken = 0;
		Timer damageTimer;
	};

	bool spriteIsVisible() const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	void onCollision(SideType side, bool isDeltaDirection);
	void onDelta(SideType side);

	ParticleTools &particleTools;

	Kinematics kinematicsX;
	Kinematics kinematicsY;
	int accX = 0; //only >0, ==0, <0
	MotionType motionType() const;
	HorizontalFacing horizontalFacing = LEFT;
	VerticalFacing intendedVerticalFacing = HORIZONTAL;
	VerticalFacing verticalFacing() const
		{ return (onGround && intendedVerticalFacing == DOWN) ? HORIZONTAL : intendedVerticalFacing; }
	WalkingAnimation walkingAnimation;

	bool gunUp() const 
		{ return (motionType() == WALKING && walkingAnimation.stride() != STRIDE_MIDDLE); }

	bool onGround = true;
	bool jumping = false;
	bool interacting = false;

	Health health;
	Timer invincibleTimer;
	std::shared_ptr<DamageText> damageText;

	GunExperienceHUD gunExperienceHUD;
	PolarStar polarStar;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

