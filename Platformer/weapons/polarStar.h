#pragma once

#include "spriteState.h"
#include "util\units.h"
#include <map>
#include <memory>
#include <tuple>

class Graphics;
class Sprite;

class PolarStar
{
public:
	PolarStar(Graphics &graphics);

	void startFire(Units::Game playerX, Units::Game playerY, 
		HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp);
	void stopFire();

	void updateProjectiles(Units::MS dt);
	void draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp, Units::Game x, Units::Game y);
private:
	typedef std::tuple<HorizontalFacing, VerticalFacing> SpriteState;

	class Projectile{
	public:
		Projectile(std::shared_ptr<Sprite> sprite, HorizontalFacing horizontalDirection, VerticalFacing verticalDirection,
			Units::Game x, Units::Game y);
		bool update(Units::MS dt);
		void draw(Graphics &graphics);
	private:
		Units::Game x, y;
		Units::Game offset = 0;
		HorizontalFacing horizontalDirection;
		VerticalFacing verticalDirection;
		std::shared_ptr<Sprite> sprite;
	};

	Units::Game gunX(HorizontalFacing horizontalFacing, Units::Game playerX);
	Units::Game gunY(VerticalFacing verticalFacing, bool gunUp, Units::Game playerY);

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	std::map<SpriteState, std::shared_ptr<Sprite>> spriteMap;
	
	std::shared_ptr<Sprite> horizontalBulletSprite;
	std::shared_ptr<Sprite> verticalBulletSprite;

	std::shared_ptr<Projectile> projectileA;
	std::shared_ptr<Projectile> projectileB;
};

