#pragma once

#include "config.h"
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

	typedef double Degrees;
	typedef double AngularVelocity;

	typedef int HP;
	typedef unsigned int GunLevel;
	typedef unsigned int gunExperience;

	namespace {
		const Game tileSize = 32.0;
		const double pi = atan(1) * 4;
	}

	inline double degreesToRadians(Degrees deg){
		return deg * pi / 180.0;
	}

	inline Game sin(Degrees degrees){
		return std::sin(degreesToRadians(degrees));
	}

	inline Game cos(Degrees degrees){
		return std::cos(degreesToRadians(degrees));
	}

	inline Pixel gameToPixel(Game game){
		return Config::getGraphicsQuality() == Config::HIGH_QUALITY ?
			Pixel(round(game)) :
			Pixel(round(game) / 2);
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

	const Game halfTile = tileToGame(1) / 2.0;

}