#pragma once

#include "spriteState.h"
#include "util\units.h"
#include <map>
#include <memory>
#include <tuple>

class Graphics;
class Sprite;

class PolarStar
{
public:
	PolarStar(Graphics &graphics);
	~PolarStar();

	void draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp, Units::Game x, Units::Game y);
private:
	typedef std::tuple<HorizontalFacing, VerticalFacing> SpriteState;

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	std::map<SpriteState, std::shared_ptr<Sprite>> spriteMap;
	std::shared_ptr<Sprite> horizontalBulletSprite;
	std::shared_ptr<Sprite> verticalBulletSprite;
};

