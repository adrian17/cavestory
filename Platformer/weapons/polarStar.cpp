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

PolarStar::~PolarStar()
{
}

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp,
	Units::Game x, Units::Game y){
	if (horizontalFacing == LEFT)
		x -= Units::halfTile;
	if (verticalFacing == UP)
		y -= Units::halfTile / 2;
	if (verticalFacing == DOWN)
		y += Units::halfTile / 2;
	if (gunUp)
		y -= 2.0;
	spriteMap[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, x, y);

	Units::Game bulletX = x - Units::halfTile;
	Units::Game bulletY = y - Units::halfTile;
	if (verticalFacing == HORIZONTAL) {
		bulletY += nozzleHorizontalY;
		bulletX += horizontalFacing == LEFT ? nozzleHorizontalLeftX : nozzleHorizontalRightX;
	} else if (verticalFacing == UP) {
		bulletY += nozzleUpY;
		bulletX += horizontalFacing == LEFT ? nozzleUpLeftX : nozzleUpRightX;
	} else if (verticalFacing == DOWN) {
		bulletY += nozzleDownY;
		bulletX += horizontalFacing == LEFT ? nozzleDownLeftX : nozzleDownRightX;
	}
	if (verticalFacing == HORIZONTAL)
		horizontalBulletSprite->draw(graphics, bulletX, bulletY);
	else
		verticalBulletSprite->draw(graphics, bulletX, bulletY);
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