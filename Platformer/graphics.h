#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Texture;
struct SDL_Window;

const int windowWidth = 640;
const int windowHeight = 480;

class Graphics{
public:
	Graphics();
	~Graphics();

	void drawTexture(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);
	void flip();

	SDL_Texture *createTexture(const std::string &path);
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

};