#pragma once

#include <map>
#include <memory>

class Graphics;
class Sprite;

class Player
{
public:
	Player(Graphics &graphics, int x, int y);
	~Player();

	void update(int dt);
	void draw(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void startJump();
	void stopJump();
private:
	enum MotionType { STANDING, WALKING };
	enum HorizontalFacing { LEFT, RIGHT };

	struct SpriteState{
		SpriteState(MotionType motionType = STANDING, HorizontalFacing horizontalFacing = LEFT) :
			motionType(motionType), horizontalFacing(horizontalFacing) {};
		MotionType motionType;
		HorizontalFacing horizontalFacing;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b);
	SpriteState getSpriteState();

	struct Jump {
		void update(int dt);
		void reset();
		void reactivate() { active = timeRemaining > 0; }
		void deactivate() { active = false; }
		bool isActive() const {return active; }
	private:
		int timeRemaining = 0;
		bool active = false;
	};

	void initSprites(Graphics &graphics);

	double x, y;
	double velX=0.0, velY=0.0;
	double accX=0.0, accY=0.0;
	HorizontalFacing horizontalFacing = LEFT;
	bool onGround = true;
	Jump jump;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

