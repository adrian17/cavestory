#pragma once

#include "util\units.h"

class CollisionRectangle;
struct Kinematics;
class Map;

class MapCollidable
{
public:
	enum SideType { TOP_SIDE, BOTTOM_SIDE, LEFT_SIDE, RIGHT_SIDE };

	void updateX(const CollisionRectangle &collisionRectangle,
		Kinematics &kinematicsX, const Kinematics &kinematicsY,
		Units::MS dt, const Map &map);
	void updateY(const CollisionRectangle collisionRectangle,
		const Kinematics &kinematicsX, Kinematics &kinematicsY,
		Units::MS dt, const Map &map);

	virtual void onCollision(MapCollidable::SideType side, bool isDeltaDirection) = 0;
	virtual void onDelta(MapCollidable::SideType side) = 0;

	virtual ~MapCollidable() {}
};

