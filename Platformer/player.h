#pragma once

#include <map>
#include <memory>

class Map;
class Graphics;
class Rectangle;
class Sprite;

class Player
{
public:
	Player(Graphics &graphics, int x, int y);
	~Player();

	void update(int dt, const Map &map);
	void updateX(int dt, const Map &map);
	void updateY(int dt, const Map &map);
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
	enum MotionType { STANDING, WALKING, JUMPING, FALLING, LAST_MOTION_TYPE};
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

	Rectangle leftCollision(int delta) const;
	Rectangle rightCollision(int delta) const;
	Rectangle topCollision(int delta) const;
	Rectangle bottomCollision(int delta) const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	double x, y;
	double velX = 0.0, velY = 0.0;
	int accX = 0; //only >0, ==0, <0
	double accY = 0.0;
	HorizontalFacing horizontalFacing = LEFT;
	VerticalFacing verticalFacing = HORIZONTAL;
	bool onGround = true;
	bool jumping;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

