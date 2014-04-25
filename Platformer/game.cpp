#include "game.h"

#include "SDL.h"
#include <cstdio>

Game::Game(){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(SDL_DISABLE);
	window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	eventLoop();
}

Game::~Game(){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::eventLoop(){
	SDL_Event event;

	bool done = false;
	while (!done){
		int startTime = SDL_GetTicks();
		while (SDL_PollEvent(&event)){
			switch (event.type){
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = true;
				break;
			}
		}


		update();
		draw();

		int elapsedTime = SDL_GetTicks() - startTime;
		if (elapsedTime > 10) elapsedTime = 10;
		SDL_Delay(1000 / 60 - elapsedTime);

		//const float fps = 1000.0 / (SDL_GetTicks() - startTime);
		//printf("fps=%f\n", fps);
	}
}

void Game::update(){

}

void Game::draw(){

}