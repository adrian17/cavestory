#include "map.h"

#include "backdrop.h"
#include "util/rectangle.h"
#include "sprite/sprite.h"

using namespace tiles;

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
	Tile tile(TileType().set(WALL), sprite);

	for (auto&& vec : map->tiles){
		vec.front() = tile;
		vec.back() = tile;
	}

	for (Units::Tile col = 0; col < nCols; ++col){
		map->tiles[0][col] = tile;
		map->tiles[nRows - 1][col] = tile;
		map->tiles[nRows - 2][col] = tile;
		map->tiles[nRows - 3][col] = tile;
		map->tiles[nRows - 4][col] = tile;
	}

	map->tiles[10][5] = tile;
	map->tiles[9][4] = tile;
	map->tiles[8][7] = tile;

	map->tiles[9][15] = tile;
	map->tiles[7][15] = tile;

	std::shared_ptr<Sprite> chain_top(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(11), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));
	std::shared_ptr<Sprite> chain_middle(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(12), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));
	std::shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(13), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1)));

	map->tiles[5][5] = tile;
	map->backgroundTiles[4][5] = chain_bottom;
	map->backgroundTiles[3][5] = chain_middle;
	map->backgroundTiles[2][5] = chain_middle;
	map->backgroundTiles[1][5] = chain_top;

	map->backgroundTiles[10][13] = chain_bottom;
	map->backgroundTiles[9][13] = chain_middle;
	map->backgroundTiles[8][13] = chain_middle;
	map->backgroundTiles[7][13] = chain_middle;
	map->backgroundTiles[6][13] = chain_top;
	map->tiles[5][13] = tile;

	return map;
}

Map* Map::createSlopeTestMap(Graphics &graphics){
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

	Tile wallTile(TileType().set(WALL),
		std::shared_ptr<Sprite>(new Sprite(graphics, "stage/PrtCave.bmp",
		Units::tileToPixel(1), 0,
		Units::tileToPixel(1), Units::tileToPixel(1))));
	enum {LTT, LTS, RTS, RTT,
		  LBT, LBS, RBS, RBT, NUM_SLOPES};
	Tile slopeTiles[NUM_SLOPES];
	for (int i = 0; i < NUM_SLOPES; ++i){
		slopeTiles[i] = Tile(
			TileType().set(SLOPE).
				set(i / 2 % 2 == 0 ? LEFT_SLOPE : RIGHT_SLOPE).
				set(i / 4 == 0 ? TOP_SLOPE : BOTTOM_SLOPE).
				set((i + 1) / 2 % 2== 0 ? TALL_SLOPE : SHORT_SLOPE),
			std::shared_ptr<Sprite>(new Sprite(graphics, "stage/PrtCave.bmp",
				Units::tileToPixel(2 + i % 4), Units::tileToPixel(i / 4),
				Units::tileToPixel(1), Units::tileToPixel(1))));
	}
	Units::Tile row = 11;
	for (Units::Tile col = 0; col < nCols; ++col){
		map->tiles[row][col] = wallTile;
	}

	row -= 1;
	Units::Tile col = 0;

	map->tiles[row][col++] = wallTile;
	map->tiles[row][col++] = slopeTiles[LBT];
	map->tiles[row][col++] = slopeTiles[RBT];
	map->tiles[row][col++] = wallTile;
	map->tiles[row][col++] = slopeTiles[LBS];
	col++;
	map->tiles[row][col++] = slopeTiles[RBS];
	map->tiles[row][col++] = slopeTiles[RBT];
	map->tiles[row][col++] = wallTile;
	map->tiles[row][col++] = slopeTiles[LBT];
	map->tiles[row][col++] = slopeTiles[LBS];
	map->tiles[row][col++] = slopeTiles[RBS];
	map->tiles[row][col++] = slopeTiles[RBT];
	map->tiles[row][col] = wallTile;
	map->tiles[row - 1][col++] = slopeTiles[RBS];
	map->tiles[row][col] = wallTile;
	map->tiles[row - 1][col++] = slopeTiles[RBT];
	map->tiles[row - 1][col++] = wallTile;
	++col;
	map->tiles[row][col++] = slopeTiles[RBS];
	map->tiles[row][col++] = wallTile;

	row -= 3; col = 0;

	map->tiles[row-1][col] = wallTile;
	map->tiles[row][col++] = wallTile;
	map->tiles[row - 1][col] = wallTile;
	map->tiles[row][col++] = slopeTiles[LTT];
	map->tiles[row - 1][col] = wallTile;
	map->tiles[row][col++] = slopeTiles[LTS];
	map->tiles[row - 1][col] = wallTile;
	map->tiles[row][col++] = slopeTiles[RTS];
	map->tiles[row - 1][col] = wallTile;
	map->tiles[row][col++] = slopeTiles[RTT];
	map->tiles[row - 1][col] = wallTile;
	map->tiles[row][col++] = wallTile;

	return map;
}

std::vector<CollisionTile> Map::getCollidingTiles(const Rectangle &rectangle, sides::SideType direction) const{
	const Units::Tile firstPrimary = Units::gameToTile(rectangle.side(sides::oppositeSide(direction)));
	const Units::Tile lastPrimary = Units::gameToTile(rectangle.side(direction));
	const Units::Tile primaryIncr = sides::isMax(direction) ? 1 : -1;

	const bool horizontal = sides::isHorizontal(direction);
	const Units::Tile minS = Units::gameToTile(horizontal ? rectangle.top() : rectangle.left());
	const Units::Tile midS = Units::gameToTile(horizontal ? rectangle.centerY() : rectangle.centerX());
	const Units::Tile maxS = Units::gameToTile(horizontal ? rectangle.bottom() : rectangle.right());

	const bool positiveS = (midS - minS) < (maxS - midS);

	const Units::Tile firstSecondary = positiveS ? minS : maxS;
	const Units::Tile lastSecondary = !positiveS ? minS : maxS;
	const Units::Tile secondaryIncr = positiveS ? 1 : -1;

	std::vector<CollisionTile> collisionTiles;
	for (Units::Tile primary = firstPrimary; primary != lastPrimary + primaryIncr; primary += primaryIncr){
		for (Units::Tile secondary = firstSecondary; secondary != lastSecondary + secondaryIncr; secondary += secondaryIncr){
			const Units::Tile row = !horizontal ? primary : secondary;
			const Units::Tile col = horizontal ? primary : secondary;
			collisionTiles.push_back(CollisionTile(row, col, tiles[row][col].tileType));
		}
	}
	return collisionTiles;
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