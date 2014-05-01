#pragma once

#include "tileType.h"
#include "sideType.h"
#include "util\units.h"

#include "boost\optional.hpp"

struct CollisionTile{
	CollisionTile(Units::Tile row, Units::Tile col, tiles::TileType tileType) :
		row(row), col(col), tileType(tileType) {}

	boost::optional<Units::Game> testCollision(sides::SideType side, Units::Game position);

private:
	Units::Tile row, col;
	tiles::TileType tileType;
};