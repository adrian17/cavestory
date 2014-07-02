#include "polarStar.h"

#include "map.h"
#include "graphics.h"
#include "particle\particleSystem.h"
#include "particle\projectileNoEffectParticle.h"
#include "particle\projectileStarParticle.h"
#include "player\gunExperienceHUD.h"
#include "sprite\sprite.h"
#include <boost\optional.hpp>
#include <algorithm>

namespace {
	const std::string spritePath = "Arms.bmp";
	const int polarStarIndex = 2;
	const Units::Pixel gunWidth = Units::gameToPixel(3 * Units::halfTile);
	const Units::Pixel gunHeight = Units::gameToPixel(2 * Units::halfTile);

	const Units::Pixel horizontalOffset = 0;
	const Units::Pixel upOffset = Units::tileToPixel(2);
	const Units::Pixel downOffset = Units::tileToPixel(4);

	const Units::Pixel leftOffset = 0;
	const Units::Pixel rightOffset = Units::tileToPixel(1);

	//nozzle offset
	const Units::Game nozzleHorizontalY = 23;
	const Units::Game nozzleHorizontalLeftX = 10;
	const Units::Game nozzleHorizontalRightX = 38;

	const Units::Game nozzleUpY = 4;
	const Units::Game nozzleUpLeftX = 27;
	const Units::Game nozzleUpRightX = 21;

	const Units::Game nozzleDownY = 28;
	const Units::Game nozzleDownLeftX = 29;
	const Units::Game nozzleDownRightX = 19;

	//projectile
	const Units::Pixel projectileSourceYs[Units::maxGunLevel] = { 
		Units::tileToPixel(2), Units::tileToPixel(2), Units::tileToPixel(3) };
	const Units::Pixel horizontalProjectileSourceXs[Units::maxGunLevel] = { 
		Units::tileToPixel(8), Units::tileToPixel(10), Units::tileToPixel(8) };

	const Units::Velocity projectileVelocity = 0.6;
	const Units::Game projectileMaxOffsets[Units::maxGunLevel] = { 7 * Units::halfTile, 10 * Units::halfTile, 14 * Units::halfTile };

	const Units::Game projectileWidths[Units::maxGunLevel] = { 4.0, 8.0, 16.0 };

	const Units::HP damages[Units::maxGunLevel] = { 1, 2, 4 };
	const Units::GunExperience experiences[] = { 0, 10, 30, 40 };
}

PolarStar::PolarStar(Graphics &graphics)
{
	initSprites(graphics);
}

void PolarStar::startFire(Units::Game playerX, Units::Game playerY,
	HorizontalFacing horizontalFacing, VerticalFacing verticalFacing,
	bool gunUp, ParticleTools &particleTools)
{
	if (projectileA && projectileB) return;
	Units::Game bulletX = gunX(horizontalFacing, playerX) - Units::halfTile;
	Units::Game bulletY = gunY(verticalFacing, gunUp, playerY) - Units::halfTile;
	if (verticalFacing == HORIZONTAL) {
		bulletY += nozzleHorizontalY;
		bulletX += horizontalFacing == LEFT ? nozzleHorizontalLeftX : nozzleHorizontalRightX;
	}
	else if (verticalFacing == UP) {
		bulletY += nozzleUpY;
		bulletX += horizontalFacing == LEFT ? nozzleUpLeftX : nozzleUpRightX;
	}
	else if (verticalFacing == DOWN) {
		bulletY += nozzleDownY;
		bulletX += horizontalFacing == LEFT ? nozzleDownLeftX : nozzleDownRightX;
	}

	if(!projectileA)
		projectileA.reset(new Projectile(
			verticalFacing == HORIZONTAL ? horizontalProjectiles[currentLevel() - 1] : verticalProjectiles[currentLevel() - 1],
			horizontalFacing, verticalFacing, bulletX, bulletY, currentLevel(), particleTools));
	else if (!projectileB)
		projectileB.reset(new Projectile(
			verticalFacing == HORIZONTAL ? horizontalProjectiles[currentLevel() - 1] : verticalProjectiles[currentLevel() - 1],
			horizontalFacing, verticalFacing, bulletX, bulletY, currentLevel(), particleTools));
}

void PolarStar::stopFire(){

}

void PolarStar::collectExperience(Units::GunExperience experience){
	currentExperience = std::min(currentExperience + experience, experiences[Units::maxGunLevel]);
}

void PolarStar::loseExperience(Units::GunExperience experience){
	currentExperience = std::max(0, currentExperience - experience);
}

std::vector<std::shared_ptr< ::Projectile>> PolarStar::getProjectiles() const{
	std::vector<std::shared_ptr< ::Projectile>> projectiles;
	if(projectileA) projectiles.push_back(projectileA);
	if(projectileB) projectiles.push_back(projectileB);
	return projectiles;
}

void PolarStar::updateProjectiles(Units::MS dt, const Map &map, ParticleTools &particleTools){
	if (projectileA){
		if (projectileA->update(dt, map, particleTools) == false)
			projectileA.reset();
	}
	if (projectileB){
		if (projectileB->update(dt, map, particleTools) == false)
			projectileB.reset();
	}
}

void PolarStar::drawHUD(Graphics &graphics, GunExperienceHUD &hud){
	const Units::GunLevel level = currentLevel();
	hud.draw(graphics, level,
		currentExperience - experiences[level - 1],
		experiences[level] - experiences[level - 1]);
}

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp,
		Units::Game playerX, Units::Game playerY) const
{
	Units::Game x = gunX(horizontalFacing, playerX);
	Units::Game y = gunY(verticalFacing, gunUp, playerY);
	spriteMap.at(SpriteState(horizontalFacing, verticalFacing))->draw(graphics, x, y);
	if(projectileA)
		projectileA->draw(graphics);
	if (projectileB)
		projectileB->draw(graphics);
}

Units::Game PolarStar::gunX(HorizontalFacing horizontalFacing, Units::Game playerX) const
{
	return (horizontalFacing == LEFT) ? playerX - Units::halfTile : playerX;
}

Units::Game PolarStar::gunY(VerticalFacing verticalFacing, bool gunUp, Units::Game playerY) const
{
	Units::Game gunY = playerY;
	if (verticalFacing == UP)
		gunY -= Units::halfTile / 2;
	else if (verticalFacing == DOWN)
		gunY += Units::halfTile / 2;
	if (gunUp)
		gunY -= 2.0;
	return gunY;
}

void PolarStar::initSprites(Graphics &graphics){
	for (Units::GunLevel gunLevel = 0; gunLevel < Units::maxGunLevel; ++gunLevel){
		horizontalProjectiles[gunLevel].reset(new Sprite(graphics, "Bullet.bmp",
			horizontalProjectileSourceXs[gunLevel], projectileSourceYs[gunLevel],
			Units::tileToPixel(1), Units::tileToPixel(1)));

		verticalProjectiles[gunLevel].reset(new Sprite(graphics, "Bullet.bmp",
			horizontalProjectileSourceXs[gunLevel] + Units::tileToPixel(1), projectileSourceYs[gunLevel],
			Units::tileToPixel(1), Units::tileToPixel(1)));
	}

	for (int horizontalFacing = 0; horizontalFacing < LAST_HORIZONTAL_FACING; ++horizontalFacing){
		for (int verticalFacing = 0; verticalFacing < LAST_VERTICAL_FACING; ++verticalFacing){
			initSprite(graphics, SpriteState((HorizontalFacing)horizontalFacing, (VerticalFacing)verticalFacing));
		}
	}
}

void PolarStar::initSprite(Graphics &graphics, const SpriteState spriteState){
	HorizontalFacing spriteHorizontalFacing = std::get<0>(spriteState);
	VerticalFacing spriteVerticalFacing = std::get<1>(spriteState);

	Units::Pixel srcY = (spriteHorizontalFacing == LEFT) ? leftOffset : rightOffset;
	if (spriteVerticalFacing == HORIZONTAL) srcY += horizontalOffset;
	else if (spriteVerticalFacing == UP) srcY += upOffset;
	else if (spriteVerticalFacing == DOWN) srcY += downOffset;

	spriteMap[spriteState].reset(new Sprite(graphics, spritePath,
		polarStarIndex*gunWidth, srcY,
		gunWidth, gunHeight));
}

Units::GunLevel PolarStar::currentLevel() const{
	Units::GunLevel level = Units::maxGunLevel;
	while (currentExperience < experiences[level - 1]) --level;
	return level;
}

PolarStar::Projectile::Projectile(std::shared_ptr<Sprite> sprite, HorizontalFacing horizontalDirection, VerticalFacing verticalDirection,
	Units::Game x, Units::Game y, Units::GunLevel gunLevel, ParticleTools &particleTools) :
	x(x), y(y),
	gunLevel(gunLevel),
	horizontalDirection(horizontalDirection), verticalDirection(verticalDirection),
	sprite(sprite)
{
	particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(new ProjectileStarParticle(particleTools.graphics, x, y)));
}

bool PolarStar::Projectile::update(const Units::MS dt, const Map &map, ParticleTools &particleTools){
	offset += projectileVelocity * dt;

	const sides::SideType direction = sides::fromFacing(horizontalDirection, verticalDirection);
	const Rectangle rectangle = collisionRectangle();
	std::vector<CollisionTile> collidingTiles = map.getCollidingTiles(rectangle, direction);
	for (auto&& tile : collidingTiles){
		const sides::SideType side = sides::oppositeSide(direction);
		const Units::Game perpendicularPosition = sides::isVertical(side) ? rectangle.centerX() : rectangle.centerY();
		const Units::Game leadingPosition = rectangle.side(direction);
		const bool testSlope = true;

		const CollisionTile::TestCollisionInfo testInfo = tile.testCollision(side, perpendicularPosition, leadingPosition, testSlope);
		if (testInfo.isColliding){
			const Units::Game collisionX = sides::isVertical(side) ? perpendicularPosition : testInfo.position;
			const Units::Game collisionY = sides::isVertical(side) ? testInfo.position : perpendicularPosition;

			particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(
				new ProjectileNoEffectParticle(particleTools.graphics, collisionX - Units::halfTile, collisionY - Units::halfTile)));
			return false;
		}
	}

	if (!alive)
		return false;
	else if (offset >= projectileMaxOffsets[gunLevel - 1]){
		particleTools.frontSystem.addNewParticle(std::shared_ptr<Particle>(new ProjectileStarParticle(particleTools.graphics, getX(), getY())));
		return false;
	}
	else
		return true;
}

void PolarStar::Projectile::draw(Graphics &graphics) const{
	sprite->draw(graphics, getX(), getY());
}

Rectangle PolarStar::Projectile::collisionRectangle() const{
	const Units::Game width = (verticalDirection == HORIZONTAL ? Units::tileToGame(1) : projectileWidths[gunLevel - 1]);
	const Units::Game height = (verticalDirection != HORIZONTAL ? Units::tileToGame(1) : projectileWidths[gunLevel - 1]);
	return Rectangle(
		getX() + Units::halfTile - width / 2,
		getY() + Units::halfTile - height / 2,
		width, height);
}

Units::HP PolarStar::Projectile::contactDamage() const{
	return damages[gunLevel - 1];
}

Units::Game PolarStar::Projectile::getX() const{
	if (verticalDirection == HORIZONTAL)
		return x + (horizontalDirection == LEFT ? -offset : offset);
	return x;
}

Units::Game PolarStar::Projectile::getY() const{
	if (verticalDirection == UP)
		return y - offset;
	else if (verticalDirection == DOWN)
		return y + offset;
	return y;
}