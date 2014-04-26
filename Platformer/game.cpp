#include "game.h"

#include "graphics.h"
#include "input.h"
#include "player.h"
#include "SDL.h"
#include <cstdio>

namespace {
	const int fps = 60;
}

Game::Game(){
	SDL_Init(SDL_INIT_EVERYTHING);

	eventLoop();
}

Game::~Game(){
	SDL_Quit();
}

void Game::eventLoop(){

	Graphics graphics;
	Input input;
	SDL_Event event;

	player.reset(new Player(graphics, 320, 240));

	int lastUpdateTime = SDL_GetTicks();
	bool done = false;
	while (!done){
		const int startTime = SDL_GetTicks();
		input.beginNewFrame();

		while (SDL_PollEvent(&event)){
			switch (event.type){
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			}
		}
		if (input.wasKeyPressed(SDLK_ESCAPE)) done = true;

		const int currentTime = SDL_GetTicks();
		update(currentTime-lastUpdateTime);
		lastUpdateTime = currentTime;
		draw(graphics);

		int elapsedTime = SDL_GetTicks() - startTime;
		if (elapsedTime > 10) elapsedTime = 10;
		SDL_Delay(1000 / fps - elapsedTime);

		//const float fps = 1000.0 / (SDL_GetTicks() - startTime);
		//printf("fps=%f\n", fps);
	}
}

void Game::update(int dt){
	player->update(dt);
}

void Game::draw(Graphics &graphics){
	player->draw(graphics);
	graphics.flip();
}