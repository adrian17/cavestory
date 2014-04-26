#include "backdrop.h"

#include "graphics.h"
#include "SDL.h"

namespace {
	const int backgroundSize = 128;
}

FixedBackdrop::FixedBackdrop(const std::string &path, Graphics &graphics){
	textureID = graphics.createTexture(path);
}

void FixedBackdrop::draw(Graphics &graphics) const{
	for (int x = 0; x < windowWidth; x += backgroundSize){
		for (int y = 0; y < windowHeight; y += backgroundSize){
			SDL_Rect dest ={x, y, backgroundSize, backgroundSize };
			graphics.drawTexture(textureID, NULL, &dest);
		}
	}
}