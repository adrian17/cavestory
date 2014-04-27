#include "animatedSprite.h"

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string filePath,
	Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height,
	Units::FPS fps, Units::Frame nFrames) :
	Sprite(graphics, filePath, srcX, srcY, width, height),
	frameTime(1000/fps),
	nFrames(nFrames)
{
}

AnimatedSprite::~AnimatedSprite(){
}

void AnimatedSprite::update(Units::MS dt){
	elapsedTime += dt;
	if (elapsedTime > frameTime){
		++currentFrame;
		elapsedTime = 0;
		if (currentFrame < nFrames){
			sourceRect.x += sourceRect.w;
		} else {
			sourceRect.x -= sourceRect.w * (nFrames - 1);
			currentFrame = 0;
		}
	}
}