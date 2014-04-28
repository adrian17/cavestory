#include "polarStar.h"

#include "graphics.h"
#include "sprite\sprite.h"

namespace {
	const std::string spritePath = "content/Arms.bmp";
	const int polarStarIndex = 2;
	const Units::Pixel gunWidth = Units::gameToPixel(3 * Units::halfTile);
	const Units::Pixel gunHeight = Units::gameToPixel(2 * Units::halfTile);

	const Units::Pixel horizontalOffset = 0;
	const Units::Pixel upOffset = Units::tileToPixel(2);
	const Units::Pixel downOffset = Units::tileToPixel(4);

	const Units::Pixel leftOffset = 0;
	const Units::Pixel rightOffset = Units::tileToPixel(1);
}

PolarStar::PolarStar(Graphics &graphics)
{
	initSprites(graphics);
}

PolarStar::~PolarStar()
{
}

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, Units::Game x, Units::Game y){
	if (horizontalFacing == LEFT)
		x -= Units::halfTile;
	if (verticalFacing == UP)
		y -= Units::halfTile / 2;
	if (verticalFacing == DOWN)
		y += Units::halfTile / 2;
	spriteMap[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, x, y);
}

void PolarStar::initSprites(Graphics &graphics){
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

	spriteMap[spriteState] = std::shared_ptr<Sprite>(new Sprite(graphics, spritePath,
		polarStarIndex*gunWidth, srcY,
		gunWidth, gunHeight));
}