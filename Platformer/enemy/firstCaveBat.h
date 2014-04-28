#pragma once

#include "spriteState.h"
#include "util/rectangle.h"
#include "util/units.h"
#include <map>
#include <memory>
#include <tuple>

class Graphics;
class Sprite;

class FirstCaveBat
{
public:
	FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y);
	~FirstCaveBat();

	void update(Units::MS dt, Units::Game playerX);
	void draw(Graphics &graphics);

	Rectangle damageRectangle() const
		{ return Rectangle(x + Units::halfTile, y + Units::halfTile, 0, 0); }

	Rectangle collisionRectangle() const
		{ return Rectangle(x, y, Units::tileToGame(1), Units::tileToGame(1)); }

	Units::HP getContactDamage() const;
private:
	typedef std::tuple<HorizontalFacing> SpriteState;
	SpriteState getSpriteState() const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	Units::Game x, y;
	const Units::Game centerY;
	Units::Degrees flightAngle = 0.0;

	HorizontalFacing horizontalFacing = LEFT;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

