#include "immobileSingleLoopParticle.h"


ImmobileSingleLoopParticle::ImmobileSingleLoopParticle(Graphics &graphics, const char* spritePath,
	Units::Game x, Units::Game y,
	Units::Pixel sourceX, Units::Pixel sourceY,
	Units::Pixel sourceW, Units::Pixel sourceH,
	Units::FPS fps, Units::Frame nFrames) :
	x(x), y(y),
	sprite(graphics, spritePath, sourceX, sourceY, sourceW, sourceH, fps, nFrames)
{
}


ImmobileSingleLoopParticle::~ImmobileSingleLoopParticle()
{
}


bool ImmobileSingleLoopParticle::update(const Units::MS dt){
	sprite.update();
	return sprite.completedLoops() == 0;
}

void ImmobileSingleLoopParticle::draw(Graphics &graphics) const{
	sprite.draw(graphics, x, y);
}