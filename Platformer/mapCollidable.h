#pragma once

#include "sideType.h"
#include "tileType.h"
#include "util\units.h"
#include <boost\optional.hpp>

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
		Units::MS dt, const Map &map,
		const boost::optional<tiles::TileType> &maybeGroundTile);

	virtual void onCollision(sides::SideType side, bool isDeltaDirection, const tiles::TileType tileType) = 0;
	virtual void onDelta(sides::SideType side) = 0;

	virtual ~MapCollidable() {}
private:
	enum AxisType { X_AXIS, Y_AXIS };
	void update(const CollisionRectangle &collisionRectangle,
		const Accelerator &accelerator,
		const Kinematics &kinematicsX, const Kinematics &kinematicsY,
		Units::MS dt, const Map &map,
		Kinematics &kinematics, AxisType axis,
		const boost::optional<tiles::TileType> &maybeGroundTile);
};

