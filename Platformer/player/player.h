#pragma once

#include "floatingNumber.h"
#include "interfaces/damageable.h"
#include "physics/kinematics.h"
#include "physics/mapCollidable.h"
#include "physics/spriteState.h"
#include "physics/tileType.h"
#include "player/gunExperienceHUD.h"
#include "sprite/sprite.h"
#include "sprite/varyingWidthSprite.h"
#include "util/timer.h"
#include "util/units.h"
#include <boost/optional.hpp>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

class Map;
class Graphics;
class ParticleTools;
class Pickup;
class PolarStar;
class Projectile;
class Rectangle;

class Player : public Damageable, private MapCollidable
{
public:
	Player(Graphics &graphics, ParticleTools &particleTools, Units::Game x, Units::Game y);
	~Player();

	void update(const Units::MS dt, const Map &map);
	void updateX(const Units::MS dt, const Map &map);
	void updateY(const Units::MS dt, const Map &map);
	void draw(Graphics &graphics) const;
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

	Units::Game centerX() const { return kinematicsX.position + Units::halfTile; }
	Units::Game centerY() const { return kinematicsY.position + Units::halfTile; }
	std::shared_ptr<FloatingNumber> getDamageText() const { return damageText; }

	std::vector<std::shared_ptr<Projectile>> getProjectiles() const;

private:
	enum MotionType { STANDING, INTERACTING, WALKING, JUMPING, FALLING, LAST_MOTION_TYPE };
	enum StrideType { STRIDE_MIDDLE, STRIDE_LEFT, STRIDE_RIGHT, LAST_STRIDE_TYPE };

	typedef std::tuple<MotionType, HorizontalFacing, VerticalFacing, StrideType> SpriteState;
	SpriteState getSpriteState() const;

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
		void update(const Units::MS dt);
		void draw(Graphics &graphics) const;
		bool takeDamage(Units::HP damage);	//true if dies
		void addHealth(Units::HP health);
	private:
		void resetFillSprites();
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

	void onCollision(sides::SideType side, bool isDeltaDirection, const tiles::TileType tileType);
	void onDelta(sides::SideType side);

	ParticleTools &particleTools;

	Kinematics kinematicsX;
	Kinematics kinematicsY;
	int accX = 0; //only >0, ==0, <0
	MotionType motionType() const;
	HorizontalFacing horizontalFacing = LEFT;
	VerticalFacing intendedVerticalFacing = HORIZONTAL;
	VerticalFacing verticalFacing() const
		{ return (onGround() && intendedVerticalFacing == DOWN) ? HORIZONTAL : intendedVerticalFacing; }
	WalkingAnimation walkingAnimation;

	bool gunUp() const 
		{ return (motionType() == WALKING && walkingAnimation.stride() != STRIDE_MIDDLE); }

	boost::optional<tiles::TileType> maybeGroundTile = boost::none;
	bool jumping = false;
	bool interacting = false;

	bool onGround() const
		{ return maybeGroundTile; }

	Health health;
	Timer invincibleTimer;
	std::shared_ptr<FloatingNumber> damageText;
	FloatingNumber experienceText;

	GunExperienceHUD gunExperienceHUD;
	std::unique_ptr<PolarStar> polarStar;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

