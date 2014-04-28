#pragma once

#include "sprite.h"

#include "util\timer.h"

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(Graphics &graphics, const std::string relativePath,
		Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height,
		Units::FPS fps, Units::Frame nFrames);
	~AnimatedSprite();

	void update(Units::MS dt);
private:
	const Units::Frame nFrames;
	Units::Frame currentFrame = 0;
	Timer frameTimer;
};