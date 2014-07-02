#pragma once

#include "interfaces\projectile.h"
#include "physics\spriteState.h"
#include "util\rectangle.h"
#include "util\units.h"
#include <map>
#include <memory>
#include <tuple>
#include <vector>

class Graphics;
class GunExperienceHUD;
class Map;
class ParticleTools;
class Sprite;

class PolarStar
{
public:
	PolarStar(Graphics &graphics);

	void startFire(Units::Game playerX, Units::Game playerY, 
		HorizontalFacing horizontalFacing, VerticalFacing verticalFacing,
		bool gunUp, ParticleTools &particleTools);
	void stopFire();
	void collectExperience(Units::GunExperience experience);
	void loseExperience(Units::GunExperience experience);

	std::vector<std::shared_ptr< ::Projectile>> getProjectiles() const;
	void updateProjectiles(Units::MS dt, const Map &map, ParticleTools &particleTools);
	void drawHUD(Graphics &graphics, GunExperienceHUD &hud);
	void draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp, Units::Game x, Units::Game y) const;
private:
	typedef std::tuple<HorizontalFacing, VerticalFacing> SpriteState;

	class Projectile : public ::Projectile{
	public:
		Projectile(std::shared_ptr<Sprite> sprite, HorizontalFacing horizontalDirection, VerticalFacing verticalDirection,
			Units::Game x, Units::Game y, Units::GunLevel gunLevel, ParticleTools &particleTools);
		bool update(const Units::MS dt, const Map &map, ParticleTools &particleTools);
		void draw(Graphics &graphics) const;

		Rectangle collisionRectangle() const;
		Units::HP contactDamage() const;
		void collideWithEnemy(){ alive = false; }
	private:
		Units::Game getX() const;
		Units::Game getY() const;
		Units::Game x, y;
		Units::Game offset = 0;
		const Units::GunLevel gunLevel;
		HorizontalFacing horizontalDirection;
		VerticalFacing verticalDirection;
		std::shared_ptr<Sprite> sprite;
		bool alive = true;
	};

	Units::Game gunX(HorizontalFacing horizontalFacing, Units::Game playerX) const;
	Units::Game gunY(VerticalFacing verticalFacing, bool gunUp, Units::Game playerY) const;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	Units::GunExperience currentExperience = 29;
	Units::GunLevel currentLevel() const;

	std::map<SpriteState, std::shared_ptr<Sprite>> spriteMap;
	
	std::shared_ptr<Sprite> horizontalProjectiles[Units::maxGunLevel];
	std::shared_ptr<Sprite> verticalProjectiles[Units::maxGunLevel];

	std::shared_ptr<Projectile> projectileA;
	std::shared_ptr<Projectile> projectileB;
};

