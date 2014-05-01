#pragma once

#include "sideType.h"
#include "util\units.h"

struct Accelerator;
class CollisionRectangle;
struct Kinematics;
class Map;

class MapCollidable
{
public:
	void updateX(const CollisionRectangle &collisionRectangle,
		const Accelerator &accelerator,
		Kinematics &kinematicsX, const Kinematics &kinematicsY,
		Units::MS dt, const Map &map);
	void updateY(const CollisionRectangle &collisionRectangle,
		const Accelerator &accelerator,
		const Kinematics &kinematicsX, Kinematics &kinematicsY,
		Units::MS dt, const Map &map);

	virtual void onCollision(sides::SideType side, bool isDeltaDirection) = 0;
	virtual void onDelta(sides::SideType side) = 0;

	virtual ~MapCollidable() {}
};

