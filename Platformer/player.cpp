#include "player.h"

#include "game.h"
#include "animatedSprite.h"

Player::Player(Graphics &graphics, int x, int y):
	x(x), y(y)
{
	sprite.reset(new AnimatedSprite(graphics, "content/myChar.bmp", 0, 0, tileSize, tileSize, 15, 3));
}


Player::~Player(){
}

void Player::update(int dt){
	sprite->update(dt);
}

void Player::draw(Graphics &graphics){
	sprite->draw(graphics, x, y);
}