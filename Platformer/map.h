#pragma once

#include "util/units.h"
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
		CollisionTile(Units::Tile row, Units::Tile col, TileType tileType) :
			row(row), col(col), tileType(tileType) {}
		Units::Tile row, col;
		TileType tileType;
	};
	static Map* createTestMap(Graphics &graphics);

	std::vector<CollisionTile> getCollidingTiles(const Rectangle &rectangle) const;

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
	std::vector<std::vector<std::shared_ptr<Sprite>>> backgroundTiles;
	std::vector<std::vector<Tile>> tiles;
};

