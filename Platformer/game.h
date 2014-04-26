#pragma once

#include <memory>

class Graphics;
class Sprite;

const int tileSize = 32;

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update(int dt);
	void draw(Graphics &graphics);

	std::unique_ptr<Sprite> sprite;
};