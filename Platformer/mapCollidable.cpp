#include "mapCollidable.h"

#include "util\accelerators.h"
#include "util\collisionRectangle.h"
#include "util\kinematics.h"
#include "map.h"

namespace {
	boost::optional<Units::Game> testMapCollision(const Map &map, const Rectangle &rectangle, sides::SideType direction){
		std::vector<CollisionTile> tiles = map.getCollidingTiles(rectangle);
		for (auto && tile : tiles){
			const sides::SideType side = sides::oppositeSide(direction);
			const Units::Game position = sides::isVertical(side) ? rectangle.centerX() : rectangle.centerY();
			const boost::optional<Units::Game> maybePosition = tile.testCollision(side, position);

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
	accelerator.updateVelocity(kinematicsX, dt);

	const Units::Game delta = kinematicsX.velocity * dt;

	if (delta > 0.0){
		const sides::SideType direction = sides::RIGHT_SIDE;

		boost::optional<Units::Game> maybePosition = testMapCollision(
			map, collisionRectangle.rightCollision(kinematicsX.position, kinematicsY.position, delta), direction);
		if (maybePosition){
			kinematicsX.position = *maybePosition - collisionRectangle.boundingBox().right();
			onCollision(direction, true);
		} else {
			kinematicsX.position += delta;
			onDelta(direction);
		}

		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		maybePosition = testMapCollision(
			map, collisionRectangle.leftCollision(kinematicsX.position, kinematicsY.position, 0), oppositeDirection);
		if (maybePosition){
			kinematicsX.position = *maybePosition - collisionRectangle.boundingBox().left();
			onCollision(oppositeDirection, true);
		}
	} else {
		const sides::SideType direction = sides::LEFT_SIDE;

		boost::optional<Units::Game> maybePosition = testMapCollision(
			map, collisionRectangle.leftCollision(kinematicsX.position, kinematicsY.position, delta), direction);
		if (maybePosition){
			kinematicsX.position = *maybePosition - collisionRectangle.boundingBox().left();
			onCollision(direction, true);
		} else {
			kinematicsX.position += delta;
			onDelta(direction);
		}
		
		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		maybePosition = testMapCollision(
			map, collisionRectangle.rightCollision(kinematicsX.position, kinematicsY.position, 0), oppositeDirection);
		if (maybePosition){
			kinematicsX.position = *maybePosition - collisionRectangle.boundingBox().right();
			onCollision(oppositeDirection, false);
		}
	}

}

void MapCollidable::updateY(const CollisionRectangle &collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, Kinematics &kinematicsY,
	Units::MS dt, const Map &map)
{
	accelerator.updateVelocity(kinematicsY, dt);

	const Units::Game delta = kinematicsY.velocity * dt;

	if (delta > 0){
		const sides::SideType direction = sides::BOTTOM_SIDE;

		boost::optional<Units::Game> maybePosition = testMapCollision(
			map, collisionRectangle.bottomCollision(kinematicsX.position, kinematicsY.position, delta), direction);
		if (maybePosition){
			kinematicsY.position = *maybePosition - collisionRectangle.boundingBox().bottom();
			onCollision(direction, true);
		} else {
			kinematicsY.position += delta;
			onDelta(direction);
		}
		
		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		maybePosition = testMapCollision(
			map, collisionRectangle.topCollision(kinematicsX.position, kinematicsY.position, 0), oppositeDirection);
		if (maybePosition){
			kinematicsY.position = *maybePosition + collisionRectangle.boundingBox().top();
			onCollision(oppositeDirection, false);
		}
	} else {
		const sides::SideType direction = sides::TOP_SIDE;
		boost::optional<Units::Game> maybePosition = testMapCollision(
			map, collisionRectangle.topCollision(kinematicsX.position, kinematicsY.position, delta), direction);
		if (maybePosition){
			kinematicsY.position = *maybePosition + collisionRectangle.boundingBox().top();
			onCollision(direction, true);
		} else {
			kinematicsY.position += delta;
			onDelta(direction);
		}

		const sides::SideType oppositeDirection = sides::oppositeSide(direction);
		maybePosition = testMapCollision(
			map, collisionRectangle.bottomCollision(kinematicsX.position, kinematicsY.position, 0), oppositeDirection);
		if (maybePosition){
			kinematicsY.position = *maybePosition - collisionRectangle.boundingBox().bottom();
			onCollision(oppositeDirection, false);
		}
	}
}