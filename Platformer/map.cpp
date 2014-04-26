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

	map->foregroundSprites = std::vector<std::vector<std::shared_ptr<Sprite>>>(
		nRows, std::vector<std::shared_ptr<Sprite>>(
			nCols, std::shared_ptr<Sprite>()));

	std::shared_ptr<Sprite> sprite(new Sprite(graphics, "content/PrtCave.bmp", tileSize, 0, tileSize, tileSize));

	const int row = 11;
	for (int col = 0; col < nCols; ++col){
		map->foregroundSprites[row][col] = sprite;
	}

	return map;
}

void Map::update(int dt){
	for (size_t row = 0; row < foregroundSprites.size(); ++row){
		for (size_t col = 0; col < foregroundSprites[row].size(); ++col){
			if (foregroundSprites[row][col]){
				foregroundSprites[row][col]->update(dt);
			}
		}
	}
}

void Map::draw(Graphics &graphics){
	for (size_t row = 0; row < foregroundSprites.size(); ++row){
		for (size_t col = 0; col < foregroundSprites[row].size(); ++col){
			if (foregroundSprites[row][col]){
				foregroundSprites[row][col]->draw(graphics, col * tileSize, row * tileSize);
			}
		}
	}
}