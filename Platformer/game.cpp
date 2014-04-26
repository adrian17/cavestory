#include "game.h"

#include "graphics.h"
#include "input.h"
#include "animatedSprite.h"
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

	sprite.reset(new AnimatedSprite(graphics, "content/myChar.bmp", 0, 0, tileSize, tileSize, 15, 3));

	int lastUpdateTime = SDL_GetTicks();
	bool done = false;
	while (!done){
		const int startTime = SDL_GetTicks();
		input.BeginNewFrame();
		while (SDL_PollEvent(&event)){
			switch (event.type){
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				input.KeyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.KeyUpEvent(event);
				break;
			}
		}
		if (input.WasKeyPressed(SDLK_ESCAPE)) done = true;

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
	sprite->Update(dt);
}

void Game::draw(Graphics &graphics){
	sprite->Draw(graphics, 320, 240);
	graphics.Flip();
}