#include "graphics.h"

#include "config.h"
#include "SDL.h"

Graphics::Graphics(){
	SDL_ShowCursor(SDL_DISABLE);

	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);

	SDL_RenderSetLogicalSize(renderer, Units::tileToPixel(screenWidth), Units::tileToPixel(screenHeight));
}

Graphics::~Graphics(){
	for (auto &&kv : spriteSheets) SDL_DestroyTexture(kv.second);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Graphics::toggleFullscreen(){
	fullscreen = !fullscreen;
	SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE);
}

Graphics::TextureID Graphics::createTexture(const std::string &relativePath, const bool transparency){
	const std::string path = Config::getGraphicsQuality() == Config::HIGH_QUALITY ?
		"./content/" + relativePath :
		"./content/ogph/" + relativePath;
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

void Graphics::drawTexture(const TextureID texture, const SDL_Rect *source, const SDL_Rect *dest){
	SDL_RenderCopy(renderer, texture, source, dest);
}

void Graphics::clear(){
	SDL_RenderClear(renderer);
	//SDL_RenderFillRect(renderer, nullptr);
}

void Graphics::flip(){
	SDL_RenderPresent(renderer);
}