#include "polarStar.h"

#include "graphics.h"
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
}

PolarStar::PolarStar(Graphics &graphics)
{
	initSprites(graphics);
}

void PolarStar::startFire(Units::Game playerX, Units::Game playerY,
	HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp)
{
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
	projectile.reset(new Projectile(
		verticalFacing == HORIZONTAL ? horizontalBulletSprite : verticalBulletSprite,
		horizontalFacing, verticalFacing, bulletX, bulletY));
}

void PolarStar::stopFire(){

}

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp,
		Units::Game playerX, Units::Game playerY){
	Units::Game x = gunX(horizontalFacing, playerX);
	Units::Game y = gunY(verticalFacing, gunUp, playerY);
	spriteMap[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, x, y);
	if(projectile)
		projectile->draw(graphics);
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
	Units::Game x, Units::Game y) :
	x(x), y(y), horizontalDirection(horizontalDirection), verticalDirection(verticalDirection), sprite(sprite)
{

}

void PolarStar::Projectile::draw(Graphics &graphics){
	sprite->draw(graphics, x, y);
}