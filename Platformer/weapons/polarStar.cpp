#include "polarStar.h"

#include "graphics.h"
#include "sprite\sprite.h"

namespace {
	const std::string spritePath = "content/Arms.bmp";
	const int polarStarIndex = 3;
	const Units::Pixel gunWidth = Units::gameToPixel(3 * Units::halfTile);
	const Units::Pixel gunHeight = Units::gameToPixel(2 * Units::halfTile);
}

PolarStar::PolarStar(Graphics &graphics)
{
	initSprites(graphics);
}

PolarStar::~PolarStar()
{
}

void PolarStar::draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, Units::Game x, Units::Game y){
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
	spriteMap[spriteState] = std::shared_ptr<Sprite>(new Sprite(graphics, spritePath,
		polarStarIndex*gunWidth, gunHeight,
		gunWidth, gunHeight));
}