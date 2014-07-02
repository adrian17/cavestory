#pragma once

#include "util\units.h"
#include <memory>
#include <set>

class Graphics;
class Pickup;
class Player;
class Map;

class Pickups
{
public:
	void add(std::shared_ptr<Pickup> pickup) { pickups.insert(pickup); }
	void handleCollision(Player &player);

	void update(const Units::MS dt, const Map &map);
	void draw(Graphics &graphics);
private:
	typedef std::set<std::shared_ptr<Pickup>> PickupSet;
	PickupSet pickups;
};