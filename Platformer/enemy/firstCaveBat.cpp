#include "firstCaveBat.h"

#include "graphics.h"
#include "sprite/animatedSprite.h"
#include <cmath>

namespace {
	const Units::FPS flyFPS = 13;
	const Units::Frame nFlyFrames = 3;

	const Units::AngularVelocity angularVelocity = 120.0 / 1000.0;

	const Units::Game flightAmplitude = Units::tileToGame(5) / 2.0;

	const Units::HP contactDamage = 1;
}

FirstCaveBat::FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y) :
	x(x), y(y),
	flightCenterY(y),
	damageText(new FloatingNumber(FloatingNumber::DAMAGE))
{
	initSprites(graphics);
}

FirstCaveBat::~FirstCaveBat(){
}

bool FirstCaveBat::update(const Units::MS dt, Units::Game playerX){
	flightAngle += angularVelocity * dt;

	horizontalFacing = x + Units::halfTile > playerX ? LEFT : RIGHT;

	y = flightCenterY + flightAmplitude * std::sin(Units::degreesToRadians(flightAngle));

	sprites[getSpriteState()]->update();
	return alive;
}

void FirstCaveBat::draw(Graphics &graphics){
	sprites[getSpriteState()]->draw(graphics, x, y);
}

Units::HP FirstCaveBat::getContactDamage() const{
	return contactDamage;
}

void FirstCaveBat::initSprites(Graphics &graphics){
	for (int horizontalFacing = 0; horizontalFacing < LAST_HORIZONTAL_FACING; ++horizontalFacing){
		initSprite(graphics, SpriteState((HorizontalFacing)horizontalFacing));
	}
}

void FirstCaveBat::initSprite(Graphics &graphics, const SpriteState spriteState){
	HorizontalFacing spriteHorizontalFacing = std::get<0>(spriteState);

	Units::Tile tileY = (spriteHorizontalFacing == LEFT) ? 2 : 3;
	sprites[spriteState].reset(new AnimatedSprite(graphics, "npc/NpcCemet.bmp",
		Units::tileToPixel(2), Units::tileToPixel(tileY),
		Units::tileToPixel(1), Units::tileToPixel(1),
		flyFPS, nFlyFrames));

}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const{
	return SpriteState(horizontalFacing);
}