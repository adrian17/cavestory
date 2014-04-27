#include "map.h"

#include "backdrop.h"
#include "game.h"
#include "graphics.h"
#include "rectangle.h"
#include "sprite/sprite.h"

Map* Map::createTestMap(Graphics &graphics){
	Map *map = new Map;

	map->backdrop.reset(new FixedBackdrop("content/bkBlue.bmp", graphics));

	const int nRows = 15;
	const int nCols = 20;

	map->tiles = std::vector<std::vector<Tile>>(
		nRows, std::vector<Tile>(
			nCols, Tile()));

	map->backgroundTiles = std::vector<std::vector<std::shared_ptr<Sprite>>>(
		nRows, std::vector<std::shared_ptr<Sprite>>(
			nCols, std::shared_ptr<Sprite>()));

	std::shared_ptr<Sprite> sprite(new Sprite(graphics, "content/PrtCave.bmp", tileSize, 0, tileSize, tileSize));
	Tile tile(WALL_TILE, sprite);

	const int row = 11;
	for (int col = 0; col < nCols; ++col){
		map->tiles[row][col] = tile;
	}
	map->tiles[10][5] = tile;
	map->tiles[9][4] = tile;
	map->tiles[8][7] = tile;

	std::shared_ptr<Sprite> chain_top(new Sprite(graphics, "content/PrtCave.bmp",
		11*tileSize, 2*tileSize, tileSize, tileSize));
	std::shared_ptr<Sprite> chain_middle(new Sprite(graphics, "content/PrtCave.bmp",
		12 * tileSize, 2 * tileSize, tileSize, tileSize));
	std::shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "content/PrtCave.bmp",
		13 * tileSize, 2 * tileSize, tileSize, tileSize));

	map->backgroundTiles[10][13] = chain_bottom;
	map->backgroundTiles[9][13] = chain_middle;
	map->backgroundTiles[8][13] = chain_middle;
	map->backgroundTiles[7][13] = chain_middle;
	map->backgroundTiles[6][13] = chain_top;
	map->tiles[5][13] = tile;

	return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle &rectangle) const{
	const int firstRow = rectangle.top() / tileSize;
	const int lastRow = rectangle.bottom() / tileSize;
	const int firstCol = rectangle.left() / tileSize;
	const int lastCol = rectangle.right() / tileSize;

	std::vector<Map::CollisionTile> collisionTiles;
	for (int row = firstRow; row <= lastRow; ++row){
		for (int col = firstCol; col <= lastCol; ++col){
			collisionTiles.push_back(CollisionTile(row, col, tiles[row][col].tileType));
		}
	}
	return collisionTiles;
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

void Map::drawBackground(Graphics &graphics){
	backdrop->draw(graphics);
	for (size_t row = 0; row < backgroundTiles.size(); ++row){
		for (size_t col = 0; col < backgroundTiles[row].size(); ++col){
			if (backgroundTiles[row][col]){
				backgroundTiles[row][col]->draw(graphics, col * tileSize, row * tileSize);
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