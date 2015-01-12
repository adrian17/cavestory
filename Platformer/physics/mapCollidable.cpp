#include "mapCollidable.h"

#include "physics/accelerators.h"
#include "physics/collisionRectangle.h"
#include "physics/kinematics.h"
#include "map.h"

namespace {
	struct CollisionInfo{
		Units::Game position;
		tiles::TileType tileType;
	};
	boost::optional<CollisionInfo> testMapCollision(
		const Map &map,
		const Rectangle &rectangle,
		sides::SideType direction,
		const boost::optional<tiles::TileType> &maybeGroundTile)
	{
		std::vector<CollisionTile> tiles = map.getCollidingTiles(rectangle, direction);
		for (auto && tile : tiles){
			const sides::SideType side = sides::oppositeSide(direction);
			const Units::Game perpendicularPosition = sides::isVertical(side) ? rectangle.centerX() : rectangle.centerY();
			const Units::Game leadingPosition = rectangle.side(direction);
			const bool testSlope = sides::isVertical(side);
			const CollisionTile::TestCollisionInfo testInfo = tile.testCollision(side, perpendicularPosition, leadingPosition, testSlope);

			if (testInfo.isColliding){
				const CollisionInfo info = { testInfo.position, tile.getTileType() };
				return info;
			} else if (maybeGroundTile && direction == sides::BOTTOM_SIDE){
				const tiles::TileType tallSlope = tiles::TileType().set(tiles::SLOPE).set(tiles::TALL_SLOPE);
				if ((maybeGroundTile->test(tiles::SLOPE) && tile.getTileType()[tiles::SLOPE]) ||
					(maybeGroundTile->test(tiles::WALL) && (tallSlope & tile.getTileType()) == tallSlope))
				{
					const CollisionInfo info = { testInfo.position, tile.getTileType() };
					return info;
				}
			}
		}
		return boost::none;
	}
}

void MapCollidable::updateX(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	Kinematics &kinematicsX, const Kinematics &kinematicsY,
	Units::MS dt, const Map &map)
{
	update(collisionRectangle, accelerator, kinematicsX, kinematicsY, dt, map, kinematicsX, X_AXIS, boost::none);
}

void MapCollidable::updateY(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, Kinematics &kinematicsY,
	Units::MS dt, const Map &map,
	const boost::optional<tiles::TileType> &maybeGroundTile)
{
	update(collisionRectangle, accelerator, kinematicsX, kinematicsY, dt, map, kinematicsY, Y_AXIS, maybeGroundTile);
}

void MapCollidable::update(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, const Kinematics &kinematicsY,
	Units::MS dt, const Map &map,
	Kinematics &kinematics, AxisType axis,
	const boost::optional<tiles::TileType> &maybeGroundTile)
{
	accelerator.updateVelocity(kinematics, dt);

	const Units::Game delta = kinematics.velocity * dt;

	const sides::SideType direction = axis == X_AXIS ?
		(delta > 0 ? sides::RIGHT_SIDE : sides::LEFT_SIDE) :
		(delta > 0 ? sides::BOTTOM_SIDE : sides::TOP_SIDE);

	{
		boost::optional<CollisionInfo> maybeInfo = testMapCollision(
			map, collisionRectangle.collision(direction, kinematicsX.position, kinematicsY.position, delta), direction, maybeGroundTile);
		if (maybeInfo){
			kinematics.position = maybeInfo->position - collisionRectangle.boundingBox().side(direction);
			onCollision(direction, true, maybeInfo->tileType);
		}
		else {
			kinematics.position += delta;
			onDelta(direction);
		}
	}
	{
		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		boost::optional<CollisionInfo> maybeInfo = testMapCollision(
			map, collisionRectangle.collision(oppositeDirection, kinematicsX.position, kinematicsY.position, 0), oppositeDirection, boost::none);
		if (maybeInfo){
			kinematics.position = maybeInfo->position - collisionRectangle.boundingBox().side(oppositeDirection);
			onCollision(oppositeDirection, false, maybeInfo->tileType);
		}
	}

}