#pragma once

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
}