#pragma once

#include "sprite\sprite.h"
#include "sprite/varyingWidthSprite.h"
#include "util\timer.h"
#include "util/units.h"
#include <map>
#include <memory>

class Map;
class NumberSprite;
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

	void takeDamage();

	Rectangle damageRectangle() const;
	Units::Game centerX() const{ return x + Units::halfTile; }
private:
	enum MotionType { STANDING, INTERACTING, WALKING, JUMPING, FALLING, LAST_MOTION_TYPE};
	enum HorizontalFacing { LEFT, RIGHT, LAST_HORIZONTAL_FACING };
	enum VerticalFacing { UP, DOWN, HORIZONTAL, LAST_VERTICAL_FACING };

	struct SpriteState{
		SpriteState(MotionType motionType = STANDING, HorizontalFacing horizontalFacing = LEFT, VerticalFacing verticalFacing = HORIZONTAL) :
			motionType(motionType), horizontalFacing(horizontalFacing), verticalFacing(verticalFacing) {};
		MotionType motionType;
		HorizontalFacing horizontalFacing;
		VerticalFacing verticalFacing;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b);
	SpriteState getSpriteState();

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
	bool onGround = true;
	bool jumping = false;
	bool interacting = false;

	Health health;
	Timer invincibleTimer;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

