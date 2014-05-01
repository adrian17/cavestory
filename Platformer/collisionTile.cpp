#include "collisionTile.h"

boost::optional<Units::Game> CollisionTile::testCollision(sides::SideType side, Units::Game position){
	if (tileType[tiles::WALL]){
		if (side == sides::TOP_SIDE)
			return Units::tileToGame(row);
		if (side == sides::BOTTOM_SIDE)
			return Units::tileToGame(row + 1);
		if (side == sides::LEFT_SIDE)
			return Units::tileToGame(col);
		return Units::tileToGame(col + 1);
	}
	else
		return boost::none;
}