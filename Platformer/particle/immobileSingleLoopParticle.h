#pragma once

#include "particle/particle.h"
#include "sprite/animatedSprite.h"

class Graphics;

class ImmobileSingleLoopParticle : public Particle
{
public:
	ImmobileSingleLoopParticle(Graphics &graphics, const char* spritePath,
		Units::Game x, Units::Game y,
		Units::Pixel sourceX, Units::Pixel sourceY,
		Units::Pixel sourceW, Units::Pixel sourceH,
		Units::FPS fps, Units::Frame nFrames);
	virtual ~ImmobileSingleLoopParticle();

	bool update(const Units::MS dt);
	void draw(Graphics &graphics) const;
private:
	const Units::Game x, y;
	AnimatedSprite sprite;

};