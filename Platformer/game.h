#pragma once

struct SDL_Window;

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update();
	void draw();

	SDL_Window *window;
};