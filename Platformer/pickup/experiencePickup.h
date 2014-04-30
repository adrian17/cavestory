#pragma once

#include "mapCollidable.h"
#include "pickup\pickup.h"
#include "sprite\animatedSprite.h"
#include "util\kinematics.h"

class Graphics;

class ExperiencePickup : public Pickup, private MapCollidable
{
public:
	enum SizeType { SMALL = 0, MEDIUM = 1, LARGE = 2 };

	ExperiencePickup(Graphics &graphics, Units::Game centerX, Units::Game centerY, SizeType size);

	Rectangle collisionRectangle() const;

	bool update(Units::MS dt, const Map &map);
	void draw(Graphics &graphics);

	int value() const;
	PickupType type() const { return EXPERIENCE; }
private:

	void onCollision(SideType side, bool isDeltaDirection);
	void onDelta(SideType side){}

	Kinematics kinematicsX, kinematicsY;
	AnimatedSprite sprite;
	SizeType size;
	Timer timer;
};