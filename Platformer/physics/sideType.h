#pragma once

#include "physics/spriteState.h"

namespace sides{
	enum SideType { TOP_SIDE, BOTTOM_SIDE, LEFT_SIDE, RIGHT_SIDE };

	inline SideType oppositeSide(SideType side){
		if (side == TOP_SIDE)
			return BOTTOM_SIDE;
		if (side == BOTTOM_SIDE)
			return TOP_SIDE;
		if (side == LEFT_SIDE)
			return RIGHT_SIDE;
		return LEFT_SIDE;
	}

	inline bool isVertical(SideType side){
		return side == TOP_SIDE || side == BOTTOM_SIDE;
	}

	inline bool isHorizontal(SideType side){
		return !isVertical(side);
	}

	inline SideType fromFacing(HorizontalFacing h, VerticalFacing v){
		if (v == UP)
			return TOP_SIDE;
		if (v == DOWN)
			return BOTTOM_SIDE;
		if (h == LEFT)
			return LEFT_SIDE;
		return RIGHT_SIDE;
	}

	inline bool isMax(SideType side){
		return (side == RIGHT_SIDE || side == BOTTOM_SIDE);
	}

	inline bool isMin(SideType side){
		return !isMax(side);
	}
};