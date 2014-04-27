#pragma once

#include "sprite.h"

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(Graphics &graphics, const std::string filePath,
		Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height,
		Units::FPS fps, Units::Frame nFrames);
	~AnimatedSprite();

	void update(Units::MS dt);
private:
	const Units::MS frameTime;
	const Units::Frame nFrames;
	Units::Frame currentFrame = 0;
	Units::MS elapsedTime = 0;
};