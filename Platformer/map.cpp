#include "map.h"

#include "game.h"
#include "graphics.h"
#include "sprite/sprite.h"

Map::Map()
{
}


Map::~Map()
{
}

Map* Map::createTestMap(Graphics &graphics){
	Map *map = new Map;

	const int nRows = 15;
	const int nCols = 20;

	map->tiles = std::vector<std::vector<Tile>>(
		nRows, std::vector<Tile>(
			nCols, Tile()));

	std::shared_ptr<Sprite> sprite(new Sprite(graphics, "content/PrtCave.bmp", tileSize, 0, tileSize, tileSize));
	Tile tile(WALL_TILE, sprite);

	const int row = 11;
	for (int col = 0; col < nCols; ++col){
		map->tiles[row][col] = tile;
	}

	return map;
}

void Map::update(int dt){
	for (size_t row = 0; row < tiles.size(); ++row){
		for (size_t col = 0; col < tiles[row].size(); ++col){
			if (tiles[row][col].sprite){
				tiles[row][col].sprite->update(dt);
			}
		}
	}
}

void Map::draw(Graphics &graphics){
	for (size_t row = 0; row < tiles.size(); ++row){
		for (size_t col = 0; col < tiles[row].size(); ++col){
			if (tiles[row][col].sprite){
				tiles[row][col].sprite->draw(graphics, col * tileSize, row * tileSize);
			}
		}
	}
}