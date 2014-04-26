#pragma once

#include <memory>
#include <vector>

class Graphics;
class Sprite;

class Map
{
public:
	Map();
	~Map();

	static Map* createTestMap(Graphics &graphics);

	void update(int dt);
	void draw(Graphics &graphics);
private:
	std::vector<std::vector<std::shared_ptr<Sprite>>> foregroundSprites;
};

