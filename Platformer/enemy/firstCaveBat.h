#pragma once

#include "damageText.h"
#include "spriteState.h"
#include "interfaces\damageable.h"
#include "util/rectangle.h"
#include "util/units.h"
#include <map>
#include <memory>
#include <tuple>

class Graphics;
class Sprite;

class FirstCaveBat : public Damageable
{
public:
	FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y);
	~FirstCaveBat();

	bool update(Units::MS dt, Units::Game playerX);
	void draw(Graphics &graphics);

	Rectangle damageRectangle() const
		{ return Rectangle(x + Units::halfTile, y + Units::halfTile, 0, 0); }

	Rectangle collisionRectangle() const
		{ return Rectangle(x, y, Units::tileToGame(1), Units::tileToGame(1)); }

	Units::HP getContactDamage() const;
	void takeDamage(Units::HP damage){
		damageText->setDamage(damage);
		alive = false;
	}

	Units::Game centerX() const{ return x + Units::halfTile; }
	Units::Game centerY() const{ return y + Units::halfTile; }
	std::shared_ptr<DamageText> getDamageText() { return damageText; }
private:
	typedef std::tuple<HorizontalFacing> SpriteState;
	SpriteState getSpriteState() const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	bool alive = true;
	Units::Game x, y;
	const Units::Game flightCenterY;
	Units::Degrees flightAngle = 0.0;

	HorizontalFacing horizontalFacing = LEFT;

	std::map<SpriteState, std::unique_ptr<Sprite>> sprites;
	std::shared_ptr<DamageText> damageText;
};

