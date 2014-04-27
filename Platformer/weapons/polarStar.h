#pragma once

#include "spriteState.h"
#include "util\units.h"
#include <map>
#include <memory>

class Graphics;
class Sprite;

class PolarStar
{
public:
	PolarStar(Graphics &graphics);
	~PolarStar();

	void draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, Units::Game x, Units::Game y);
private:
	struct SpriteState{
		SpriteState(HorizontalFacing horizontalFacing = LEFT, VerticalFacing verticalFacing = HORIZONTAL) :
			horizontalFacing(horizontalFacing), verticalFacing(verticalFacing) {};
		HorizontalFacing horizontalFacing;
		VerticalFacing verticalFacing;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b){
		if (a.horizontalFacing != b.horizontalFacing)
			return a.horizontalFacing < b.horizontalFacing;
		return a.verticalFacing < b.verticalFacing;
	}

	void initSprites(Graphics &graphics);
	void initSprite(Graphics &graphics, const SpriteState spriteState);

	std::map<SpriteState, std::shared_ptr<Sprite>> spriteMap;
};

