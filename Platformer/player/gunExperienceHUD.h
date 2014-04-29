#pragma once

#include "sprite\sprite.h"
#include "sprite\varyingWidthSprite.h"
#include "util\timer.h"

class Graphics;

class GunExperienceHUD
{
public:
	GunExperienceHUD(Graphics &graphics);

	void activateFlash() { flashTimer.reset(); }
	void draw(Graphics &graphics, Units::GunLevel gunLevel, Units::GunLevel currentExperience, Units::GunLevel levelExperience);
private:
	Sprite experienceBarSprite;
	Sprite levelSprite;
	Sprite flashSprite;
	Sprite maxSprite;
	VaryingWidthSprite fillSprite;

	Timer flashTimer;
};

