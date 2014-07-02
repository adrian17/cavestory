#include "flashingPickup.h"

namespace {
	const std::string spritePath = "npc/NpcSym.bmp";

	const Units::Tile dissipatingSpriteX = 1;
	const Units::Tile dissipatingSpriteY = 0;

	const Units::Tile heartSourceX = 2;
	const Units::Tile heartSourceY = 5;
	const Rectangle heartRectangle = { 5, 8, 25, 19 };
	const Units::HP heartValue = 2;

	const Units::Tile multiHeartSourceX = 4;
	const Units::Tile multiHeartSourceY = 5;
	const Rectangle multiHeartRectangle = { 6, 7, 26, 25 };
	const Units::HP multiHeartValue = 6;

	const Units::Tile heartsSourceX = 1;
	const Units::Tile heartsSourceY = 0;

	const Units::MS lifetime = 8000;
	const Units::MS flashPeriod = 60;
	const Units::MS flickerTime = 7000;
	const Units::MS flickerPeriod = 50;
	const Units::MS dissipateTime = lifetime - 25;
}

Rectangle FlashingPickup::collisionRectangle() const{
	return Rectangle(
		x + rectangle.left(), y + rectangle.top(),
		rectangle.width(), rectangle.height());
}

bool FlashingPickup::update(const Units::MS dt, const Map &map){
	return timer.active();
}

void FlashingPickup::draw(Graphics &graphics){
	Units::MS time = timer.getCurrentTime();

	if (time > dissipateTime){
		dissipatingSprite.draw(graphics, x, y);
	}
	else if (time < flickerTime || time / flickerPeriod % 2 == 0) {
		if (time / flashPeriod % 2 == 0)
			sprite.draw(graphics, x, y);
		else
			flashSprite.draw(graphics, x, y);
	}
}

std::shared_ptr<Pickup> FlashingPickup::HeartPickup(Graphics &graphics, Units::Game centerX, Units::Game centerY){
	return std::shared_ptr<Pickup>(
		new FlashingPickup(graphics, centerX, centerY, heartSourceX, heartSourceY, heartRectangle, heartValue, Pickup::HEALTH));
}

std::shared_ptr<Pickup> FlashingPickup::MultiHeartPickup(Graphics &graphics, Units::Game centerX, Units::Game centerY){
	return std::shared_ptr<Pickup>(
		new FlashingPickup(graphics, centerX, centerY, multiHeartSourceX, multiHeartSourceY, multiHeartRectangle, multiHeartValue, Pickup::HEALTH));
}

FlashingPickup::FlashingPickup(Graphics &graphics,
	Units::Game centerX, Units::Game centerY,
	Units::Tile sourceX, Units::Tile sourceY,
	const Rectangle &rectangle,
	const int value, const PickupType type) :
	sprite(graphics, spritePath, Units::tileToPixel(sourceX), Units::tileToPixel(sourceY), Units::tileToPixel(1), Units::tileToPixel(1)),
	flashSprite(graphics, spritePath, Units::tileToPixel(sourceX + 1), Units::tileToPixel(sourceY), Units::tileToPixel(1), Units::tileToPixel(1)),
	dissipatingSprite(graphics, spritePath, Units::tileToPixel(dissipatingSpriteX), Units::tileToPixel(dissipatingSpriteY), Units::tileToPixel(1), Units::tileToPixel(1)),
	x(centerX - Units::halfTile), y(centerY - Units::halfTile),
	timer(lifetime, true), rectangle(rectangle), value_(value), type_(type)
{}