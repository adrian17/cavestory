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
	enum TileType { AIR_TILE, WALL_TILE };
	struct Tile{
		Tile(TileType tileType = AIR_TILE, std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>()) :
			tileType(tileType), sprite(sprite) {}
		TileType tileType;
		std::shared_ptr<Sprite> sprite;
	};
	std::vector<std::vector<Tile>> tiles;
};

