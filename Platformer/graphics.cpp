#include "graphics.h"

#include "config.h"
#include "SDL.h"

Graphics::Graphics(){
	SDL_ShowCursor(SDL_DISABLE);

	//do not use: in fullscreen, it uses 640x480 anyway.
	//SDL_CreateWindowAndRenderer(Units::tileToPixel(screenWidth), Units::tileToPixel(screenHeight), 0, &window, &renderer);

	SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
	//SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_FULLSCREEN, &window, &renderer); //fullscreen
}

Graphics::~Graphics(){
	for (auto &&kv : spriteSheets) SDL_DestroyTexture(kv.second);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

Graphics::TextureID Graphics::createTexture(const std::string &relativePath, bool transparency){
	const std::string path = Config::getGraphicsQuality() == Config::HIGH_QUALITY ?
		"content/" + relativePath :
		"content/ogph/" + relativePath;
	if (spriteSheets.count(path) == false){
		SDL_Surface *tempSurface = SDL_LoadBMP(path.c_str());
		if (!tempSurface){
			std::string errorString = relativePath + " is missing";
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
				"Missing file",
				errorString.c_str(),
				NULL);
			exit(1);
		}
		if (transparency){
			const Uint32 blackColor = SDL_MapRGB(tempSurface->format, 0, 0, 0);
			SDL_SetColorKey(tempSurface, SDL_TRUE, blackColor);
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		spriteSheets[path] = texture;
	}
	return spriteSheets[path];
}

void Graphics::drawTexture(TextureID texture, SDL_Rect *source, SDL_Rect *dest){
	SDL_Rect newDest = (*dest);
	if (Config::getGraphicsQuality() == Config::LOW_QUALITY){
		newDest.x *= 2; newDest.y *= 2;
		newDest.w *= 2; newDest.h *= 2;
	}
	SDL_RenderCopy(renderer, texture, source, &newDest);
}

void Graphics::clear(){
	SDL_RenderFillRect(renderer, nullptr);
}

void Graphics::flip(){
	SDL_RenderPresent(renderer);
}