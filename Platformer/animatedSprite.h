#pragma once

#include "sprite.h"

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(Graphics &graphics, const std::string filePath,
		int srcX, int srcY, int width, int height, int fps, int nFrames);
	~AnimatedSprite();

	void update(int dt);
private:
	const int frameTime;
	const int nFrames;
	int currentFrame;
	int elapsedTime;
};