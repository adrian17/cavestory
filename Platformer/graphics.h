#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Texture;
struct SDL_Window;

class Graphics{
public:
	Graphics();
	~Graphics();

	void DrawTexture(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);
	void Flip();

	SDL_Texture *CreateTexture(const std::string &path);
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

};