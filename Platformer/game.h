#pragma once

#include <memory>

class Graphics;
class Sprite;

const int fps = 60;

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update();
	void draw(Graphics &graphics);

	std::unique_ptr<Sprite> sprite;
};