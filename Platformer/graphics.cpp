#include "graphics.h"

#include "SDL.h"

Graphics::Graphics(){
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_FULLSCREEN, &window, &renderer);
	SDL_CreateWindowAndRenderer(windowWidth, windowHeight, 0, &window, &renderer);
}

Graphics::~Graphics(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

SDL_Texture* Graphics::createTexture(const std::string &path){
	SDL_Surface *tempSurface = SDL_LoadBMP(path.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return texture;
}

void Graphics::drawTexture(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest){
	SDL_RenderCopy(renderer, texture, source, dest);
}

void Graphics::flip(){
	SDL_RenderPresent(renderer);
}