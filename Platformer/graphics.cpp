#include "graphics.h"

#include "SDL.h"

Graphics::Graphics(){
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_CreateWindowAndRenderer(Units::tileToPixel(screenWidth), Units::tileToPixel(screenHeight), SDL_WINDOW_FULLSCREEN, &window, &renderer);
	SDL_CreateWindowAndRenderer(Units::tileToPixel(screenWidth), Units::tileToPixel(screenHeight), 0, &window, &renderer);
}

Graphics::~Graphics(){
	for (auto &&kv : spriteSheets) SDL_DestroyTexture(kv.second);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

Graphics::TextureID Graphics::createTexture(const std::string &path, bool transparency){
	if (spriteSheets.count(path) == false){
		SDL_Surface *tempSurface = SDL_LoadBMP(path.c_str());
		if (transparency)
			SDL_SetColorKey(tempSurface, SDL_TRUE, 0);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		spriteSheets[path] = texture;
	}
	return spriteSheets[path];
}

void Graphics::drawTexture(TextureID texture, SDL_Rect *source, SDL_Rect *dest){
	SDL_RenderCopy(renderer, texture, source, dest);
}

void Graphics::clear(){
	SDL_RenderFillRect(renderer, nullptr);
}

void Graphics::flip(){
	SDL_RenderPresent(renderer);
}