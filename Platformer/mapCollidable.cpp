#include "mapCollidable.h"

#include "util\accelerators.h"
#include "util\collisionRectangle.h"
#include "util\kinematics.h"
#include "map.h"

namespace {
	struct CollisionInfo{
		bool collided;
		Units::Tile row, col;
	} info;

	CollisionInfo getWallCollisionInfo(const Map &map, const Rectangle &rectangle){
		CollisionInfo info = { false, 0, 0 };
		std::vector<Map::CollisionTile> tiles = map.getCollidingTiles(rectangle);
		for (auto && tile : tiles){
			if (tile.tileType == Map::WALL_TILE){
				info = { true, tile.row, tile.col };
				break;
			}
		}
		return info;
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
		CollisionInfo info = getWallCollisionInfo(map, collisionRectangle.rightCollision(kinematicsX.position, kinematicsY.position, delta));
		if (info.collided){
			kinematicsX.position = Units::tileToGame(info.col) - collisionRectangle.boundingBox().right();
			onCollision(MapCollidable::RIGHT_SIDE, true);
		} else {
			kinematicsX.position += delta;
			onDelta(MapCollidable::RIGHT_SIDE);
		}
		//other dir
		info = getWallCollisionInfo(map, collisionRectangle.leftCollision(kinematicsX.position, kinematicsY.position, 0));
		if (info.collided){
			kinematicsX.position = Units::tileToGame(info.col + 1) - collisionRectangle.boundingBox().left();
			onCollision(MapCollidable::LEFT_SIDE, true);
		}
	} else {
		CollisionInfo info = getWallCollisionInfo(map, collisionRectangle.leftCollision(kinematicsX.position, kinematicsY.position, delta));
		if (info.collided){
			kinematicsX.position = Units::tileToGame(info.col + 1) - collisionRectangle.boundingBox().left();
			onCollision(MapCollidable::LEFT_SIDE, true);
		} else {
			kinematicsX.position += delta;
			onDelta(MapCollidable::LEFT_SIDE);
		}
		//other dir
		info = getWallCollisionInfo(map, collisionRectangle.rightCollision(kinematicsX.position, kinematicsY.position, 0));
		if (info.collided){
			kinematicsX.position = Units::tileToGame(info.col) - collisionRectangle.boundingBox().right();
			onCollision(MapCollidable::RIGHT_SIDE, false);
		}
	}

}

void MapCollidable::updateY(const CollisionRectangle collisionRectangle,
	const Accelerator &accelerator,
	const Kinematics &kinematicsX, Kinematics &kinematicsY,
	Units::MS dt, const Map &map)
{
	accelerator.updateVelocity(kinematicsY, dt);

	const Units::Game delta = kinematicsY.velocity * dt;

	if (delta > 0){
		CollisionInfo info = getWallCollisionInfo(map, collisionRectangle.bottomCollision(kinematicsX.position, kinematicsY.position, delta));
		if (info.collided){
			kinematicsY.position = Units::tileToGame(info.row) - collisionRectangle.boundingBox().bottom();
			onCollision(MapCollidable::BOTTOM_SIDE, true);
		} else {
			kinematicsY.position += delta;
			onDelta(MapCollidable::BOTTOM_SIDE);
		}
		//other dir
		info = getWallCollisionInfo(map, collisionRectangle.topCollision(kinematicsX.position, kinematicsY.position, 0));
		if (info.collided){
			kinematicsY.position = Units::tileToGame(info.row + 1) + collisionRectangle.boundingBox().top();
			onCollision(MapCollidable::TOP_SIDE, false);
		}
	} else {
		CollisionInfo info = getWallCollisionInfo(map, collisionRectangle.topCollision(kinematicsX.position, kinematicsY.position, delta));
		if (info.collided){
			kinematicsY.position = Units::tileToGame(info.row + 1) + collisionRectangle.boundingBox().top();
			onCollision(MapCollidable::TOP_SIDE, true);
		} else {
			kinematicsY.position += delta;
			onDelta(MapCollidable::TOP_SIDE);
		}
		//other dir
		info = getWallCollisionInfo(map, collisionRectangle.bottomCollision(kinematicsX.position, kinematicsY.position, 0));
		if (info.collided){
			kinematicsY.position = Units::tileToGame(info.row) - collisionRectangle.boundingBox().bottom();
			onCollision(MapCollidable::BOTTOM_SIDE, false);
		}
	}
}