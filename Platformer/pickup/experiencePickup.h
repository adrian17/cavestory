#pragma once

#include "physics\kinematics.h"
#include "physics\mapCollidable.h"
#include "pickup\pickup.h"
#include "sprite\animatedSprite.h"

class Graphics;

class ExperiencePickup : public Pickup, private MapCollidable
{
public:
	enum SizeType { SMALL = 0, MEDIUM = 1, LARGE = 2 };

	ExperiencePickup(Graphics &graphics, Units::Game centerX, Units::Game centerY, SizeType size);

	Rectangle collisionRectangle() const;

	bool update(const Units::MS dt, const Map &map);
	void draw(Graphics &graphics) const;

	int value() const;
	PickupType type() const { return EXPERIENCE; }
private:

	void onCollision(sides::SideType side, bool isDeltaDirection, const tiles::TileType tileType);
	void onDelta(sides::SideType side){}

	Kinematics kinematicsX, kinematicsY;
	AnimatedSprite sprite;
	SizeType size;
	Timer timer;
};