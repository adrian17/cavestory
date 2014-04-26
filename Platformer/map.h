#pragma once

#include <memory>
#include <vector>

class Backdrop;
class Graphics;
class Rectangle;
class Sprite;

class Map
{
public:
	enum TileType { AIR_TILE, WALL_TILE };
	struct CollisionTile{
		CollisionTile(int row, int col, TileType tileType) :
			row(row), col(col), tileType(tileType) {}
		int row, col;
		TileType tileType;
	};
	static Map* createTestMap(Graphics &graphics);

	std::vector<CollisionTile> getCollidingTiles(const Rectangle &rectangle) const;

	void update(int dt);
	void drawBackground(Graphics &graphics);
	void draw(Graphics &graphics);
private:
	struct Tile{
		Tile(TileType tileType = AIR_TILE, std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>()) :
			tileType(tileType), sprite(sprite) {}
		TileType tileType;
		std::shared_ptr<Sprite> sprite;
	};
	std::shared_ptr<Backdrop> backdrop;
	std::vector<std::vector<Tile>> tiles;
};

