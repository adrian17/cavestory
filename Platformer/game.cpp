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
				if (event.key.repeat != 0) break;
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			}
		}
		if (input.wasKeyPressed(SDLK_ESCAPE)) done = true;

		if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT)) player->stopMoving();
		else if (input.isKeyHeld(SDLK_LEFT)) player->startMovingLeft();
		else if (input.isKeyHeld(SDLK_RIGHT)) player->startMovingRight();
		else player->stopMoving();

		if (input.isKeyHeld(SDLK_UP) && input.isKeyHeld(SDLK_DOWN)) player->lookHorizontal();
		else if (input.isKeyHeld(SDLK_UP)) player->loopUp();
		else if (input.isKeyHeld(SDLK_DOWN)) player->lookDown();
		else player->lookHorizontal();

		if (input.wasKeyPressed(SDLK_z)) player->startJump();
		else if (input.wasKeyReleased(SDLK_z)) player->stopJump();

		const int currentTime = SDL_GetTicks();
		update(currentTime-lastUpdateTime);
		lastUpdateTime = currentTime;
		draw(graphics);

		const int msPerFrame = 1000 / fps;
		const int elapsedTime = SDL_GetTicks() - startTime;
		if(elapsedTime < msPerFrame)
			SDL_Delay(1000 / fps - elapsedTime);

		//const float fps = 1000.0 / (SDL_GetTicks() - startTime);
		//printf("fps=%f\n", fps);
	}
}

void Game::update(int dt){
	player->update(dt);
}

void Game::draw(Graphics &graphics){
	graphics.clear();
	player->draw(graphics);
	graphics.flip();
}