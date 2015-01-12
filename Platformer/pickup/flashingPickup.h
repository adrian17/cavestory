#pragma once

#include "pickup/pickup.h"
#include "sprite/sprite.h"
#include "util/timer.h"
#include <memory>

class FlashingPickup : public Pickup
{
public:
	Rectangle collisionRectangle() const;
	bool update(const Units::MS dt, const Map &map);
	void draw(Graphics &graphics) const;

	int value() const { return value_; }
	PickupType type() const { return type_; }

	static std::shared_ptr<Pickup> HeartPickup(Graphics &graphics, Units::Game centerX, Units::Game centerY);
	static std::shared_ptr<Pickup> MultiHeartPickup(Graphics &graphics, Units::Game centerX, Units::Game centerY);

private:
	FlashingPickup(Graphics &graphics,
		Units::Game centerX, Units::Game centerY,
		Units::Tile sourceX, Units::Tile sourceY,
		const Rectangle &rectangle,
		const int value, const PickupType type);

	Sprite sprite, flashSprite, dissipatingSprite;

	Units::Game x, y;

	Timer timer;

	const Rectangle &rectangle;
	const int value_;
	const PickupType type_;
};