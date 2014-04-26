#pragma once

#include <map>
#include <string>

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Texture;
struct SDL_Window;

const int windowWidth = 640;
const int windowHeight = 480;

class Graphics{
public:
	typedef SDL_Texture* TextureID;

	Graphics();
	~Graphics();

	void drawTexture(TextureID texture, SDL_Rect *source, SDL_Rect *dest);
	void clear();
	void flip();

	TextureID createTexture(const std::string &path);
private:
	std::map<std::string, TextureID> spriteSheets;

	SDL_Window *window;
	SDL_Renderer *renderer;
};