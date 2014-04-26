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
private:
	int x, y;
	std::unique_ptr<Sprite> sprite;
};

