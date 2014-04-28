#pragma once

#include "damageText.h"
#include "spriteState.h"
#include "sprite\sprite.h"
#include "sprite/varyingWidthSprite.h"
#include "util\timer.h"
#include "util/units.h"
#include "weapons\polarStar.h"
#include <map>
#include <memory>
#include <tuple>

class Map;
class Graphics;
class Rectangle;

class Player
{
public:
	Player(Graphics &graphics, Units::Game x, Units::Game y);
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

	void takeDamage(Units::HP damage);

	Rectangle damageRectangle() const;
	Units::Game centerX() const{ return x + Units::halfTile; }
	Units::Game centerY() const{ return y + Units::halfTile; }
private:
	enum MotionType { STANDING, INTERACTING, WALKING, JUMPING, FALLING, LAST_MOTION_TYPE };
	enum StrideType { STRIDE_MIDDLE, STRIDE_LEFT, STRIDE_RIGHT, LAST_STRIDE_TYPE };

	typedef std::tuple<MotionType, HorizontalFacing, VerticalFacing, StrideType> SpriteState;
	SpriteState getSpriteState();

	struct WalkingAnimation{
		WalkingAnimation();
		StrideType stride() const;
		void update();
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
		Units::Game fillOffset(Units::HP health) const;
		Units::HP currentHealth = 6;
		Units::HP maxHealth = 6;
		Sprite healthBarSprite;
		VaryingWidthSprite healthFillSprite;
		VaryingWidthSprite damageFillSprite;

		Units::HP damageTaken = 0;
		Timer damageTimer;
	};

	Rectangle leftCollision(Units::Game delta) const;
	Rectangle rightCollision(Units::Game delta) const;
	Rectangle topCollision(Units::Game delta) const;
	Rectangle bottomCollision(Units::Game delta) const;

	bool spriteIsVisible() const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	Units::Game x, y;
	Units::Velocity velX = 0.0, velY = 0.0;
	int accX = 0; //only >0, ==0, <0
	HorizontalFacing horizontalFacing = LEFT;
	VerticalFacing verticalFacing = HORIZONTAL;
	WalkingAnimation walkingAnimation;
	bool onGround = true;
	bool jumping = false;
	bool interacting = false;

	Health health;
	Timer invincibleTimer;
	DamageText damageText;

	PolarStar polarStar;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

