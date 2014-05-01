#pragma once

#include "sideType.h"

#include <bitset>

namespace tiles{
	enum TileFlag{
		EMPTY,
		WALL,
		SLOPE,

		LEFT_SLOPE,
		RIGHT_SLOPE,

		TOP_SLOPE,
		BOTTOM_SLOPE,

		TALL_SLOPE,
		SHORT_SLOPE,

		LAST_TILE_FLAG
	};

	typedef std::bitset<LAST_TILE_FLAG> TileType;
	inline TileFlag slopeFlagFromSide(sides::SideType side){
		if (side == sides::LEFT_SIDE)
			return LEFT_SLOPE;
		if (side == sides::RIGHT_SIDE)
			return RIGHT_SLOPE;
		if (side == sides::TOP_SIDE)
			return TOP_SLOPE;
		return BOTTOM_SLOPE;
	}
}