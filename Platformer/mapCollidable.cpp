#include "mapCollidable.h"

#include "util\accelerators.h"
#include "util\collisionRectangle.h"
#include "util\kinematics.h"
#include "map.h"

namespace {
	boost::optional<Units::Game> testMapCollision(const Map &map, const Rectangle &rectangle, sides::SideType direction){
		std::vector<CollisionTile> tiles = map.getCollidingTiles(rectangle, direction);
		for (auto && tile : tiles){
			const sides::SideType side = sides::oppositeSide(direction);
			const Units::Game perpendicularPosition = sides::isVertical(side) ? rectangle.centerX() : rectangle.centerY();
			const Units::Game leadingPosition = rectangle.side(direction);
			const bool testSlope = sides::isVertical(side);
			const boost::optional<Units::Game> maybePosition = tile.testCollision(side, perpendicularPosition, leadingPosition, testSlope);

			if (maybePosition)
				return maybePosition;
		}
		return boost::none;
	}
}

void MapCollidable::updateX(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	Kinematics &kinematicsX, const Kinematics &kinematicsY,
	Units::MS dt, const Map &map)
{
	update(collisionRectangle, accelerator, kinematicsX, kinematicsY, dt, map, kinematicsX, X_AXIS);
}

void MapCollidable::updateY(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, Kinematics &kinematicsY,
	Units::MS dt, const Map &map)
{
	update(collisionRectangle, accelerator, kinematicsX, kinematicsY, dt, map, kinematicsY, Y_AXIS);
}

void MapCollidable::update(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, const Kinematics &kinematicsY,
	Units::MS dt, const Map &map,
	Kinematics &kinematics, AxisType axis)
{
	accelerator.updateVelocity(kinematics, dt);

	const Units::Game delta = kinematics.velocity * dt;

	const sides::SideType direction = axis == X_AXIS ?
		(delta > 0 ? sides::RIGHT_SIDE : sides::LEFT_SIDE) :
		(delta > 0 ? sides::BOTTOM_SIDE : sides::TOP_SIDE);

	{
		boost::optional<Units::Game> maybePosition = testMapCollision(
			map, collisionRectangle.collision(direction, kinematicsX.position, kinematicsY.position, delta), direction);
		if (maybePosition){
			kinematics.position = *maybePosition - collisionRectangle.boundingBox().side(direction);
			onCollision(direction, true);
		}
		else {
			kinematics.position += delta;
			onDelta(direction);
		}
	}
	{
		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		boost::optional<Units::Game>maybePosition = testMapCollision(
			map, collisionRectangle.collision(oppositeDirection, kinematicsX.position, kinematicsY.position, 0), oppositeDirection);
		if (maybePosition){
			kinematics.position = *maybePosition - collisionRectangle.boundingBox().side(oppositeDirection);
			onCollision(oppositeDirection, false);
		}
	}

}