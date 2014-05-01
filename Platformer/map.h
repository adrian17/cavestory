#pragma once

#include "collisionTile.h"
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
	static Map* createTestMap(Graphics &graphics);

	std::vector<CollisionTile> getCollidingTiles(const Rectangle &rectangle) const;

	void drawBackground(Graphics &graphics);
	void draw(Graphics &graphics);
private:
	struct Tile{
		Tile(tiles::TileType tileType = tiles::TileType().set(tiles::EMPTY), std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>()) :
			tileType(tileType), sprite(sprite) {}
		tiles::TileType tileType;
		std::shared_ptr<Sprite> sprite;
	};
	std::shared_ptr<Backdrop> backdrop;
	std::vector<std::vector<std::shared_ptr<Sprite>>> backgroundTiles;
	std::vector<std::vector<Tile>> tiles;
};

