#include "game.h"

#include "graphics.h"
#include "sprite.h"
#include "SDL.h"
#include <cstdio>

Game::Game(){
	SDL_Init(SDL_INIT_EVERYTHING);

	eventLoop();
}

Game::~Game(){
	SDL_Quit();
}

void Game::eventLoop(){

	Graphics graphics;

	SDL_Event event;

	sprite.reset(new Sprite(graphics, "content/myChar.bmp", 0, 0, 32, 32));

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
		draw(graphics);

		int elapsedTime = SDL_GetTicks() - startTime;
		if (elapsedTime > 10) elapsedTime = 10;
		SDL_Delay(1000 / fps - elapsedTime);

		//const float fps = 1000.0 / (SDL_GetTicks() - startTime);
		//printf("fps=%f\n", fps);
	}
}

void Game::update(){

}

void Game::draw(Graphics &graphics){
	sprite->Draw(graphics, 320, 240);
	graphics.Flip();
}