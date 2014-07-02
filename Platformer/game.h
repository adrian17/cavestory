#pragma once

#include "damageTexts.h"
#include "particle\particleSystem.h"
#include "pickup\pickups.h"
#include "util\units.h"
#include <memory>

class FirstCaveBat;
class Graphics;
class Map;
class Player;
class Sprite;

class Game{
public:
	Game();
	~Game();
private:
	void eventLoop();

	void update(const Units::MS dt, Graphics &graphics);
	void draw(Graphics &graphics);

	std::shared_ptr<Player> player;
	std::unique_ptr<Map> map;
	std::shared_ptr<FirstCaveBat> bat;
	DamageTexts damageTexts;
	ParticleSystem frontParticleSystem;
	ParticleSystem entityParticleSystem;
	Pickups pickups;
};