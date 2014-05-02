#include "collisionTile.h"

using namespace sides;
using namespace tiles;

namespace {
	inline double getSlope(TileType tileType){
		const TileType rightTop = TileType().set(RIGHT_SLOPE).set(TOP_SLOPE);
		const TileType leftBottom = TileType().set(LEFT_SLOPE).set(BOTTOM_SLOPE);
		const bool isPositive = ((rightTop & tileType) == rightTop || (leftBottom & tileType) == leftBottom);
		return isPositive ? 0.5 : -0.5;
	}

	inline Units::Game getOffset(TileType tileType){
		{
			const TileType leftTopTall = TileType().set(LEFT_SLOPE).set(TOP_SLOPE).set(TALL_SLOPE);
			const TileType rightBottomShort = TileType().set(RIGHT_SLOPE).set(BOTTOM_SLOPE).set(SHORT_SLOPE);
			if ((leftTopTall & tileType) == leftTopTall || (rightBottomShort & tileType) == rightBottomShort)
				return Units::tileToGame(1);
		}
		{
			const TileType leftBottomTall = TileType().set(LEFT_SLOPE).set(BOTTOM_SLOPE).set(TALL_SLOPE);
			const TileType rightTopShort = TileType().set(RIGHT_SLOPE).set(TOP_SLOPE).set(SHORT_SLOPE);
			if ((leftBottomTall & tileType) == leftBottomTall || (rightTopShort & tileType) == rightTopShort)
				return 0;
		}
		return Units::halfTile;
	}
}

CollisionTile::TestCollisionInfo CollisionTile::testCollision(SideType side,
	Units::Game perpendicularPosition, Units::Game leadingPosition, bool testSlope){
	TestCollisionInfo info = { false, leadingPosition };
	if (tileType[WALL]){
		info.isColliding = true;
		if (side == TOP_SIDE)
			info.position = Units::tileToGame(row);
		else if (side == BOTTOM_SIDE)
			info.position = Units::tileToGame(row + 1);
		else if (side == LEFT_SIDE)
			info.position = Units::tileToGame(col);
		else
			info.position = Units::tileToGame(col + 1);
	}
	else if (testSlope && tileType[SLOPE] && !tileType[slopeFlagFromSide(side)]){
		Units::Game gameRow = Units::tileToGame(row);
		Units::Game gameCol = Units::tileToGame(col);
		const double slope = getSlope(tileType);
		const Units::Game offset = getOffset(tileType);

		const Units::Game calculatedPosition = sides::isVertical(side) ?
			slope * (perpendicularPosition - gameCol) + offset + gameRow :
			(perpendicularPosition - gameRow - offset) / slope + gameCol;

		const bool isColliding = isMax(side) ?
			leadingPosition <= calculatedPosition :
			leadingPosition >= calculatedPosition;
		info.isColliding = isColliding;
		info.position = calculatedPosition;
	}
	return info;
}