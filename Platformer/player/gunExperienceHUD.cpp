#include "gunExperienceHUD.h"

#include "sprite\numberSprite.h"

namespace {
	const std::string spritePath = "TextBox.bmp";
	Units::Game drawY = 3 * Units::halfTile;

	Units::Pixel levelSourceX = Units::tileToPixel(5);
	Units::Pixel levelSourceY = Units::tileToPixel(5);
	Units::Pixel levelSourceW = Units::tileToPixel(1);
	Units::Pixel levelSourceH = Units::gameToPixel(Units::halfTile);

	Units::Game levelDrawX = Units::tileToGame(1);
	Units::Game levelNumberDrawX = 3 * Units::halfTile;

	Units::Pixel experienceBarSourceX = 0;
	Units::Pixel experienceSourceY = Units::gameToPixel(9 * Units::halfTile);
	Units::Pixel experienceSourceW = Units::gameToPixel(5 * Units::halfTile);
	Units::Pixel experienceSourceH = Units::gameToPixel(1 * Units::halfTile);

	Units::Game experienceBarDrawX = 5 * Units::halfTile;

	Units::Pixel flashBarSourceX = Units::gameToPixel(5 * Units::halfTile);
	Units::Pixel flashBarSourceY = Units::tileToPixel(5);

	Units::Pixel fillSourceX = 0;
	Units::Pixel fillSourceY = Units::tileToPixel(5);

	Units::Pixel maxSourceX = Units::gameToPixel(5 * Units::halfTile);
	Units::Pixel maxSourceY = Units::gameToPixel(9 * Units::halfTile);

	const Units::MS flashTime = 800;
	const Units::MS flashPeroid = 40;
}

GunExperienceHUD::GunExperienceHUD(Graphics &graphics) :
	experienceBarSprite(graphics, spritePath, experienceBarSourceX, experienceSourceY, experienceSourceW, experienceSourceH),
	levelSprite(graphics, spritePath, levelSourceX, levelSourceY, levelSourceW, levelSourceH),
	flashSprite(graphics, spritePath, flashBarSourceX, flashBarSourceY, experienceSourceW, experienceSourceH),
	maxSprite(graphics, spritePath, maxSourceX, maxSourceY, experienceSourceW, experienceSourceH),
	fillSprite(graphics, spritePath, fillSourceX, fillSourceY, experienceSourceW, 0, experienceSourceH),
	flashTimer(flashTime)
{
}

void GunExperienceHUD::draw(Graphics &graphics, Units::GunLevel gunLevel, Units::GunLevel currentExperience, Units::GunLevel levelExperience){
	levelSprite.draw(graphics, levelDrawX, drawY);
	NumberSprite::HUDNumber(graphics, gunLevel, 2).draw(graphics, levelNumberDrawX, drawY);

	experienceBarSprite.draw(graphics, experienceBarDrawX, drawY);

	if (currentExperience < levelExperience){
		fillSprite.setPercentageWidth(currentExperience * 1.0 / levelExperience);
		fillSprite.draw(graphics, experienceBarDrawX, drawY);
	}
	else
		maxSprite.draw(graphics, experienceBarDrawX, drawY);

	if(flashTimer.active() && flashTimer.getCurrentTime() / flashPeroid % 2 == 0)
		flashSprite.draw(graphics, experienceBarDrawX, drawY);
}