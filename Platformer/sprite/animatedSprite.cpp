#include "animatedSprite.h"

AnimatedSprite::AnimatedSprite(Graphics &graphics, const char* relativePath,
	Units::Pixel srcX, Units::Pixel srcY, Units::Pixel width, Units::Pixel height,
	Units::FPS fps, Units::Frame nFrames) :
	Sprite(graphics, relativePath, srcX, srcY, width, height),
	nFrames(nFrames),
	frameTimer(1000/fps)
{
}

AnimatedSprite::~AnimatedSprite(){
}

void AnimatedSprite::update(){
	if (frameTimer.expired()){
		frameTimer.reset();
		++currentFrame;
		if (currentFrame < nFrames){
			sourceRect.x += sourceRect.w;
		} else {
			sourceRect.x -= sourceRect.w * (nFrames - 1);
			currentFrame = 0;
			nCompletedLoops++;
		}
	}
}