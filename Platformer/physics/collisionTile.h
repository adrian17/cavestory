#pragma once

#include "physics/sideType.h"
#include "physics/tileType.h"
#include "util/units.h"

struct CollisionTile{
	CollisionTile(Units::Tile row, Units::Tile col, tiles::TileType tileType) :
		row(row), col(col), tileType(tileType) {}

	struct TestCollisionInfo{
		bool isColliding;
		Units::Game position;
	};

	TestCollisionInfo testCollision(sides::SideType side,
		Units::Game perpendicularPosition, Units::Game leadingPosition, bool testSlope);

	tiles::TileType getTileType() const{ return tileType; }

private:
	Units::Tile row, col;
	tiles::TileType tileType;
};