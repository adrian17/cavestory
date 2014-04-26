#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

class Graphics{
public:
	Graphics();
	~Graphics();

	void Graphics::DrawTexture(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *dest);

private:
	SDL_Window *window;
	SDL_Renderer *renderer;

};