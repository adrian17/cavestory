#pragma once

#include "util\units.h"
#include <memory>

class FirstCaveBat;
class Graphics;
class Map;
class Player;
class Sprite;

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update(Units::MS dt);
	void draw(Graphics &graphics);

	std::unique_ptr<Player> player;
	std::unique_ptr<Map> map;
	std::unique_ptr<FirstCaveBat> bat;
};