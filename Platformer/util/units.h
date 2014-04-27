#pragma once

#include <cmath>

namespace Units {
	typedef double Game;
	typedef int Pixel;
	typedef unsigned int Tile;

	typedef unsigned int Frame;	//nFrames

	typedef unsigned int MS;	//miliseconds
	typedef unsigned int FPS;

	typedef double Velocity;
	typedef double Acceleration;

	namespace {
		const Game tileSize = 32.0;
	}

	inline Pixel gameToPixel(Game game){
		return Pixel(round(game));
	}

	inline Tile gameToTile(Game game){
		return Tile(game / tileSize);
	}

	inline Game tileToGame(Tile tile){
		return tile * tileSize;
	}

	inline Pixel tileToPixel(Tile tile){
		return gameToPixel(tileToGame(tile));
	}

}