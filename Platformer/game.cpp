#include "game.h"

#include "graphics.h"
#include "input.h"
#include "map.h"
#include "enemy\firstCaveBat.h"
#include "player/player.h"
#include "util\timer.h"
#include "SDL.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
//#include <cstdio>

namespace {
	const Units::FPS fps = 60;
	const Units::MS maxFrameTime = 5 * 1000 / 60;
}

Game::Game(){
	SDL_Init(SDL_INIT_EVERYTHING);
	srand((unsigned int)time(nullptr));
	eventLoop();
}

Game::~Game(){
	SDL_Quit();
}

void Game::eventLoop(){

	Graphics graphics;
	Input input;
	SDL_Event event;

	player.reset(new Player(graphics, Units::tileToGame(screenWidth / 2), Units::tileToGame(screenHeight / 2)));
	damageTexts.addDamageable(player);
	map.reset(Map::createTestMap(graphics));
	bat.reset(new FirstCaveBat(graphics, Units::tileToGame(8), Units::tileToGame(screenHeight / 2+1)));
	damageTexts.addDamageable(bat);

	Units::MS lastUpdateTime = SDL_GetTicks();
	bool done = false;
	while (!done){
		const Units::MS startTime = SDL_GetTicks();
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

		ParticleTools particleTools = { particleSystem, graphics };
		if (input.wasKeyPressed(SDLK_x)) player->startFire(particleTools);
		else if (input.wasKeyReleased(SDLK_x)) player->stopFire();

		const Units::MS currentTime = SDL_GetTicks();
		const Units::MS dt = currentTime - lastUpdateTime;
		update(std::min(dt, maxFrameTime), graphics);
		lastUpdateTime = currentTime;
		draw(graphics);

		const Units::MS msPerFrame = 1000 / fps;
		const Units::MS elapsedTime = SDL_GetTicks() - startTime;
		if(elapsedTime < msPerFrame)
			SDL_Delay(1000 / fps - elapsedTime);

		//const float fps = 1000.0 / (SDL_GetTicks() - startTime);
		//printf("fps=%f\n", fps);
	}
}

void Game::update(Units::MS dt, Graphics &graphics){
	Timer::updateAll(dt);
	particleSystem.update(dt);
	damageTexts.update(dt);

	ParticleTools particleTools = {particleSystem, graphics};
	player->update(dt, *map, particleTools);
	if (bat){
		if (bat->update(dt, player->centerX()) == false)
			bat.reset();
	}
	
	for (auto&& projectile : player->getProjectiles()){
		if (bat && bat->collisionRectangle().collidesWith(projectile->collisionRectangle())){
			projectile->collideWithEnemy();
			if (bat)
				bat->takeDamage(projectile->contactDamage());
		}
	}

	if (bat && bat->damageRectangle().collidesWith(player->damageRectangle()))
		player->takeDamage(bat->getContactDamage());
}

void Game::draw(Graphics &graphics){
	graphics.clear();
	map->drawBackground(graphics);
	if (bat)
		bat->draw(graphics);
	player->draw(graphics);
	map->draw(graphics);
	particleSystem.draw(graphics);
	damageTexts.draw(graphics);
	player->drawHUD(graphics);
	graphics.flip();
}