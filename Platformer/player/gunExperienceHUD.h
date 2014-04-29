#pragma once

#include "sprite\sprite.h"
#include "util\timer.h"

class Graphics;

class GunExperienceHUD
{
public:
	GunExperienceHUD(Graphics &graphics);

	void activateFlash() { flashTimer.reset(); }
	void draw(Graphics &graphics, Units::GunLevel gunLevel);
private:
	Sprite experienceBarSprite;
	Sprite levelSprite;
	Sprite flashSprite;

	Timer flashTimer;
};

