#pragma once

#include "util/units.h"
#include <map>
#include <memory>

class Map;
class Graphics;
class Rectangle;
class Sprite;

class Player
{
public:
	Player(Graphics &graphics, Units::Game x, Units::Game y);
	~Player();

	void update(Units::MS dt, const Map &map);
	void updateX(Units::MS dt, const Map &map);
	void updateY(Units::MS dt, const Map &map);
	void draw(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void loopUp();
	void lookDown();
	void lookHorizontal();

	void startJump();
	void stopJump();
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

	Rectangle leftCollision(Units::Game delta) const;
	Rectangle rightCollision(Units::Game delta) const;
	Rectangle topCollision(Units::Game delta) const;
	Rectangle bottomCollision(Units::Game delta) const;

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

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

