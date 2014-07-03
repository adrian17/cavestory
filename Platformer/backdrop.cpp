#include "backdrop.h"

#include "graphics.h"
#include "SDL_rect.h"

namespace {
	const Units::Tile backgroundSize = 4;
}

FixedBackdrop::FixedBackdrop(const char* path, Graphics &graphics){
	textureID = graphics.createTexture(path);
}

void FixedBackdrop::draw(Graphics &graphics) const{
	for (Units::Tile x = 0; x < screenWidth; x += backgroundSize){
		for (Units::Tile y = 0; y < screenHeight; y += backgroundSize){
			SDL_Rect dest = { Units::tileToPixel(x), Units::tileToPixel(y),
				Units::tileToPixel(backgroundSize), Units::tileToPixel(backgroundSize) };
			graphics.drawTexture(textureID, nullptr, &dest);
		}
	}
}