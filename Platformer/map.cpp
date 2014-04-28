#include "map.h"

#include "backdrop.h"
#include "util/rectangle.h"
#include "sprite/sprite.h"

Map* Map::createTestMap(Graphics &graphics){
	Map *map = new Map;

	map->backdrop.reset(new FixedBackdrop("bkBlue.bmp", graphics));

	const Units::Tile nRows = 15;
	const Units::Tile nCols = 20;

	map->tiles = std::vector<std::vector<Tile>>(
		nRows, std::vector<Tile>(
			nCols, Tile()));

	map->backgroundTiles = std::vector<std::vector<std::shared_ptr<Sprite>>>(
		nRows, std::vector<std::shared_ptr<Sprite>>(
			nCols, std::shared_ptr<Sprite>()));

	std::shared_ptr<Sprite> sprite(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(1), 0,
		Units::tileToPixel(1), Units::tileToPixel(1)));
	Tile tile(WALL_TILE, sprite);

	const Units::Tile row = 11;
	for (Units::Tile col = 0; col < nCols; ++col){
		map->tiles[row][col] = tile;
	}
	map->tiles[10][5] = tile;
	map->tiles[9][4] = tile;
	map->tiles[8][7] = tile;

	std::shared_ptr<Sprite> chain_top(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(11), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));
	std::shared_ptr<Sprite> chain_middle(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(12), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));
	std::shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(13), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));

	map->backgroundTiles[10][13] = chain_bottom;
	map->backgroundTiles[9][13] = chain_middle;
	map->backgroundTiles[8][13] = chain_middle;
	map->backgroundTiles[7][13] = chain_middle;
	map->backgroundTiles[6][13] = chain_top;
	map->tiles[5][13] = tile;

	return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle &rectangle) const{
	const Units::Tile firstRow = Units::gameToTile(rectangle.top());
	const Units::Tile lastRow = Units::gameToTile(rectangle.bottom());
	const Units::Tile firstCol = Units::gameToTile(rectangle.left());
	const Units::Tile lastCol = Units::gameToTile(rectangle.right());

	std::vector<Map::CollisionTile> collisionTiles;
	for (Units::Tile row = firstRow; row <= lastRow; ++row){
		for (Units::Tile col = firstCol; col <= lastCol; ++col){
			collisionTiles.push_back(CollisionTile(row, col, tiles[row][col].tileType));
		}
	}
	return collisionTiles;
}

void Map::update(Units::MS dt){
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
				backgroundTiles[row][col]->draw(graphics, Units::tileToGame(col), Units::tileToGame(row));
			}
		}
	}
}

void Map::draw(Graphics &graphics){
	for (size_t row = 0; row < tiles.size(); ++row){
		for (size_t col = 0; col < tiles[row].size(); ++col){
			if (tiles[row][col].sprite){
				tiles[row][col].sprite->draw(graphics, Units::tileToGame(col), Units::tileToGame(row));
			}
		}
	}
}