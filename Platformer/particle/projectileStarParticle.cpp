#include "projectileStarParticle.h"

namespace {
	const std::string spritePath = "Caret.bmp";
	const Units::Pixel sourceX = Units::tileToPixel(0);
	const Units::Pixel sourceY = Units::tileToPixel(3);
	const Units::Pixel sourceW = Units::tileToPixel(1);
	const Units::Pixel sourceH = Units::tileToPixel(1);

	const Units::FPS fps = 18;
	const Units::Frame nFrames = 4;
}

ProjectileStarParticle::ProjectileStarParticle(Graphics &graphics, Units::Game x, Units::Game y) :
	ImmobileSingleLoopParticle(graphics, spritePath, x, y, sourceX, sourceY, sourceW, sourceH, fps, nFrames)
{

}