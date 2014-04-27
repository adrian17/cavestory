#pragma once

#include "util/rectangle.h"
#include "util/units.h"
#include <map>
#include <memory>

class Graphics;
class Sprite;

class FirstCaveBat
{
public:
	FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y);
	~FirstCaveBat();

	void update(Units::MS dt, Units::Game playerX);
	void draw(Graphics &graphics);

	Rectangle damageRectangle() const{
		return Rectangle(x+Units::tileToGame(1) / 2.0, y + Units::tileToGame(1) / 2, 0, 0);
	}
private:
	enum HorizontalFacing { LEFT, RIGHT, LAST_HORIZONTAL_FACING };
	struct SpriteState{
		SpriteState(HorizontalFacing horizontalFacing = LEFT) :
			horizontalFacing(horizontalFacing){};
		HorizontalFacing horizontalFacing;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b){
		return a.horizontalFacing < b.horizontalFacing;
	}
	SpriteState getSpriteState() const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	Units::Game x, y;
	const Units::Game centerY;
	Units::Degrees flightAngle = 0.0;

	HorizontalFacing horizontalFacing = LEFT;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
};

