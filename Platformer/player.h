#pragma once

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
private:
	double x, y;
	double velX=0.0, velY=0.0;
	double accX=0.0, accY=0.0;
	std::unique_ptr<Sprite> sprite;
};

