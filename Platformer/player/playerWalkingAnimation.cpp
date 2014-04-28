#include "player.h"

namespace {
	const Units::MS walkFps = 15;
	const Units::Frame nWalkFrames = 3;
}

Player::WalkingAnimation::WalkingAnimation() :
	frameTimer(1000/walkFps)
{}

Player::StrideType Player::WalkingAnimation::stride() const{
	if (currentFrame == 0) return STRIDE_LEFT;
	else if (currentFrame == 1) return STRIDE_MIDDLE;
	else if (currentFrame == 2) return STRIDE_RIGHT;
	else return STRIDE_MIDDLE;
}

void Player::WalkingAnimation::update(){
	if (frameTimer.expired()){
		frameTimer.reset();

		if (forward){
			currentFrame++;
			if (currentFrame == nWalkFrames - 1)
				forward = false;
		} else {
			currentFrame--;
			if (currentFrame == 0)
				forward = true;
		}
	}
}

void Player::WalkingAnimation::reset(){
	forward = true;
	currentFrame = 0;
	frameTimer.reset();
}