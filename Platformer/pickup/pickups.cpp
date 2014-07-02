#include "pickups.h"

#include "player\player.h"
#include "pickup\pickup.h"

void Pickups::handleCollision(Player &player){
	for (PickupSet::iterator iter = pickups.begin(); iter != pickups.end();){
		if (player.damageRectangle().collidesWith((*iter)->collisionRectangle())){
			player.collectPickup(**iter);
			pickups.erase(iter++);
		}
		else
			++iter;
	}
}

void Pickups::update(const Units::MS dt, const Map &map){
	for (PickupSet::iterator iter = pickups.begin(); iter != pickups.end();){
		if ((*iter)->update(dt, map))
			++iter;
		else
			pickups.erase(iter++);
	}
}
void Pickups::draw(Graphics &graphics){
	for (auto&& pickup : pickups)
		pickup->draw(graphics);
}