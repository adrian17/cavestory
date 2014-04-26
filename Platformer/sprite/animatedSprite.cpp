#include "animatedSprite.h"

#include "../graphics.h"
#include "../game.h"

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string filePath,
	int srcX, int srcY, int width, int height, int fps, int nFrames) :
	Sprite(graphics, filePath, srcX, srcY, width, height),
	frameTime(1000/fps),
	nFrames(nFrames)
{
}

AnimatedSprite::~AnimatedSprite(){
}

void AnimatedSprite::update(int dt){
	elapsedTime += dt;
	if (elapsedTime > frameTime){
		++currentFrame;
		elapsedTime = 0;
		if (currentFrame < nFrames){
			sourceRect.x += tileSize;
		} else {
			sourceRect.x -= tileSize * (nFrames - 1);
			currentFrame = 0;
		}
	}
}