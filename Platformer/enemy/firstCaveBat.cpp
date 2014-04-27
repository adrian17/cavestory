#include "firstCaveBat.h"

#include "graphics.h"
#include "sprite/animatedSprite.h"
#include <cmath>

namespace {
	const Units::FPS flyFPS = 13;
	const Units::Frame nFlyFrames = 3;

	const Units::AngularVelocity angularVelocity = 120.0 / 1000.0;
}

FirstCaveBat::FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y) :
	x(x), y(y)
{
	initSprites(graphics);
}

FirstCaveBat::~FirstCaveBat(){
}

void FirstCaveBat::update(Units::MS dt, Units::Game playerX){
	flightAngle += angularVelocity * dt;

	horizontalFacing = x + Units::tileToGame(1) / 2.0 > playerX ? LEFT : RIGHT;

	sprites[getSpriteState()]->update(dt);
}

void FirstCaveBat::draw(Graphics &graphics){
	const Units::Game drawY = y + Units::tileToGame(5) / 2.0 * std::sin(Units::degreesToRadians(flightAngle));
	sprites[getSpriteState()]->draw(graphics, x, drawY);
}

void FirstCaveBat::initSprites(Graphics &graphics){
	for (int horizontalFacing = 0; horizontalFacing < LAST_HORIZONTAL_FACING; ++horizontalFacing){
		initSprite(graphics, SpriteState((HorizontalFacing)horizontalFacing));
	}
}

void FirstCaveBat::initSprite(Graphics &graphics, const SpriteState spriteState){
	Units::Tile tileY = (spriteState.horizontalFacing == LEFT) ? 2 : 3;
	sprites[spriteState] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics, "content/npc/NpcCemet.bmp",
		Units::tileToPixel(2), Units::tileToPixel(tileY),
		Units::tileToPixel(1), Units::tileToPixel(1),
		flyFPS, nFlyFrames));

}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const{
	return SpriteState(horizontalFacing);
}