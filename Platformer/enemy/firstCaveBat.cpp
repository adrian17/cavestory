#include "firstCaveBat.h"

#include "graphics.h"
#include "sprite/animatedSprite.h"

namespace {
	const Units::FPS flyFPS = 10;
	const Units::Frame nFlyFrames = 3;
}

FirstCaveBat::FirstCaveBat(Graphics &graphics, Units::Game x, Units::Game y) :
	x(x), y(y)
{
	sprite.reset(new AnimatedSprite(graphics, "content/npc/NpcCemet.bmp",
		Units::tileToPixel(2), Units::tileToPixel(2),
		Units::tileToPixel(1), Units::tileToPixel(1),
		flyFPS, nFlyFrames));
}

FirstCaveBat::~FirstCaveBat(){
}

void FirstCaveBat::update(Units::MS dt){
	sprite->update(dt);
}

void FirstCaveBat::draw(Graphics &graphics) const{
	sprite->draw(graphics, x, y);
}