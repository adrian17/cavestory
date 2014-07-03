#include "player.h"

#include "sprite/numberSprite.h"

#include <algorithm>

namespace {
	const char* spritePath = "TextBox.bmp";

	const Units::Game healthBarX = Units::tileToGame(1);
	const Units::Game healthBarY = Units::tileToGame(2);

	const Units::Pixel healthBarSourceX = 0;
	const Units::Pixel healthBarSourceY = Units::gameToPixel(5 * Units::halfTile);
	const Units::Pixel healthBarSourceW = Units::tileToPixel(4);
	const Units::Pixel healthBarSourceH = Units::gameToPixel(Units::halfTile);

	const Units::Game healthFillX = 5 * Units::halfTile;
	const Units::Game healthFillY = Units::tileToGame(2);

	const Units::Pixel maxFillWidth = Units::gameToPixel(5 * Units::halfTile - 2.0);

	const Units::Pixel healthDamageSourceX = 0;
	const Units::Pixel healthDamageSourceY = Units::tileToPixel(2);
	//const Units::Pixel healthDamageSourceW = 0; <- not used, changes
	const Units::Pixel healthDamageSourceH = Units::gameToPixel(Units::halfTile);;

	const Units::Pixel healthFillSourceX = 0;
	const Units::Pixel healthFillSourceY = Units::gameToPixel(3 * Units::halfTile);
	//const Units::Pixel healthFillSourceW <- not used, chagnes
	const Units::Pixel healthFillSourceH = Units::gameToPixel(Units::halfTile);

	const Units::Game healthNumberX = Units::tileToGame(3) / 2.0;
	const Units::Game healthNumberY = Units::tileToGame(2);
	const int numDigits = 2;

	const Units::MS damageDelay = 1500;
}

Player::Health::Health(Graphics &graphics) :
	healthBarSprite(graphics, spritePath,
		healthBarSourceX, healthBarSourceY,
		healthBarSourceW, healthBarSourceH),
	healthFillSprite(graphics, spritePath,
		healthFillSourceX, healthFillSourceY,
		maxFillWidth,
		Units::gameToPixel(maxFillWidth), healthFillSourceH),
	damageFillSprite(graphics, spritePath,
		healthDamageSourceX, healthDamageSourceY,
		maxFillWidth,
		0, healthDamageSourceH),
		damageTimer(damageDelay)
{

}


void Player::Health::update(const Units::MS dt){
	if (damageTaken > 0 && damageTimer.expired()){
		currentHealth -= damageTaken;
		damageTaken = 0;
	}
}

void Player::Health::draw(Graphics &graphics) const{
	healthBarSprite.draw(graphics, healthBarX, healthBarY);

	if (damageTaken > 0){
		damageFillSprite.draw(graphics, healthFillX, healthFillY);
	}

	healthFillSprite.draw(graphics, healthFillX, healthFillY);

	NumberSprite::HUDNumber(graphics, currentHealth, numDigits).draw(graphics, healthNumberX, healthNumberY);
}

bool Player::Health::takeDamage(Units::HP damage){
	damageTaken = damage;
	damageTimer.reset();
	resetFillSprites();
	return false;
}

void Player::Health::addHealth(Units::HP health){
	if (damageTaken > health) {
		damageTaken -= health;
		health = 0;
	} else if (damageTaken > 0){
		health -= damageTaken;
		damageTaken = 0;
	}

	currentHealth = std::min(currentHealth + health, maxHealth);
	resetFillSprites();
}

void Player::Health::resetFillSprites(){
	healthFillSprite.setPercentageWidth((currentHealth - damageTaken) * 1.0 / maxHealth);
	damageFillSprite.setPercentageWidth(currentHealth * 1.0 / maxHealth);
}