#include "polarStar.h"

#include "map.h"
#include "graphics.h"
#include "particle\particleSystem.h"
#include "particle\projectileNoEffectParticle.h"
#include "particle\projectileStarParticle.h"
#include "sprite\sprite.h"

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
	const Units::Pixel projectileSourceY = Units::tileToPixel(2);
	const Units::Pixel horizontalProjectileSourceX = Units::tileToPixel(8);
	const Units::Pixel verticalProjectileSourceX = Units::tileToPixel(9);

	const Units::Velocity projectileVelocity = 0.6;
	const Units::Game projectileMaxOffset = 7 * Units::halfTile;

	const Units::Game projectileWidth = 4.0;
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
			verticalFacing == HORIZONTAL ? horizontalBulletSprite : verticalBulletSprite,
			horizontalFacing, verticalFacing, bulletX, bulletY, particleTools));
	else if (!projectileB)
		projectileB.reset(new Projectile(
			verticalFacing == HORIZONTAL ? horizontalBulletSprite : verticalBulletSprite,
			horizontalFacing, verticalFacing, bulletX, bulletY, particleTools));
}

void PolarStar::stopFire(){

}

std::vector<std::shared_ptr< ::Projectile>> PolarStar::getProjectiles(){
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

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp,
		Units::Game playerX, Units::Game playerY){
	Units::Game x = gunX(horizontalFacing, playerX);
	Units::Game y = gunY(verticalFacing, gunUp, playerY);
	spriteMap[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, x, y);
	if(projectileA)
		projectileA->draw(graphics);
	if (projectileB)
		projectileB->draw(graphics);
}

Units::Game PolarStar::gunX(HorizontalFacing horizontalFacing, Units::Game playerX)
{ 
	return (horizontalFacing == LEFT) ? playerX - Units::halfTile : playerX;
}

Units::Game PolarStar::gunY(VerticalFacing verticalFacing, bool gunUp, Units::Game playerY){
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
	horizontalBulletSprite.reset(new Sprite(graphics, "Bullet.bmp",
		horizontalProjectileSourceX, projectileSourceY,
		Units::tileToPixel(1), Units::tileToPixel(1)));

	verticalBulletSprite.reset(new Sprite(graphics, "Bullet.bmp",
		verticalProjectileSourceX, projectileSourceY,
		Units::tileToPixel(1), Units::tileToPixel(1)));

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

PolarStar::Projectile::Projectile(std::shared_ptr<Sprite> sprite, HorizontalFacing horizontalDirection, VerticalFacing verticalDirection,
	Units::Game x, Units::Game y, ParticleTools &particleTools) :
	x(x), y(y), horizontalDirection(horizontalDirection), verticalDirection(verticalDirection), sprite(sprite)
{
	particleTools.system.addNewParticle(std::shared_ptr<Particle>(new ProjectileStarParticle(particleTools.graphics, x, y)));
}

bool PolarStar::Projectile::update(Units::MS dt, const Map &map, ParticleTools &particleTools){
	offset += projectileVelocity * dt;

	std::vector<Map::CollisionTile> collidingTiles = map.getCollidingTiles(collisionRectangle());
	for (auto&& tile : collidingTiles){
		if (tile.tileType == Map::WALL_TILE){
			const Rectangle tileRect(
				Units::tileToGame(tile.col), Units::tileToGame(tile.row),
				Units::tileToGame	(1), Units::tileToGame(1));
			Units::Game particleX, particleY;
			if (verticalDirection == HORIZONTAL){
				particleX = (horizontalDirection == LEFT) ? tileRect.right() : tileRect.left();
				particleX -= Units::halfTile;
				particleY = getY();
			} else {
				particleY = (verticalDirection == UP) ? tileRect.bottom() : tileRect.top();
				particleY -= Units::halfTile;
				particleX = getX();
			}
			particleTools.system.addNewParticle(std::shared_ptr<Particle>(
				new ProjectileNoEffectParticle(particleTools.graphics, particleX, particleY)));
			return false;
		}
	}

	if (!alive)
		return false;
	else if (offset >= projectileMaxOffset){
		particleTools.system.addNewParticle(std::shared_ptr<Particle>(new ProjectileStarParticle(particleTools.graphics, getX(), getY())));
		return false;
	}
	else
		return true;
}

void PolarStar::Projectile::draw(Graphics &graphics){
	sprite->draw(graphics, getX(), getY());
}

Rectangle PolarStar::Projectile::collisionRectangle() const{
	const Units::Game width = (verticalDirection == HORIZONTAL ? Units::tileToGame(1) : projectileWidth);
	const Units::Game height = (verticalDirection != HORIZONTAL ? Units::tileToGame(1) : projectileWidth);
	return Rectangle(
		getX() + Units::halfTile - width / 2,
		getY() + Units::halfTile - height / 2,
		width, height);
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