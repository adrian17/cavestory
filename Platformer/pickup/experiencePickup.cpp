#include "experiencePickup.h"

#include "physics\accelerators.h"
#include "physics\collisionRectangle.h"
#include <cstdlib>

namespace {
	const std::string spritePath = "npc/npcSym.bmp";

	const Units::Pixel sourceX = 0;
	const Units::Pixel sourceYs[] = { Units::tileToPixel(1), Units::tileToPixel(2), Units::tileToPixel(3) };
	const Units::Pixel sourceW = Units::tileToPixel(1);
	const Units::Pixel sourceH = Units::tileToPixel(1);

	const Units::FPS fps = 14;
	const Units::Frame nFrames = 6;

	const Units::MS lifetime = 8000;
	const Units::MS flickerTime = 7000;
	const Units::MS flickerPeriod = 50;

	const Units::GunExperience values[] = { 1, 5, 20 };

	const Units::Velocity bounceVelocity = 0.225;

	const FrictionAccelerator frictionAccelerator(0.00002);
	const SimpleCollisionRectangle collisionRectangles[] = {
		SimpleCollisionRectangle(Rectangle(12, 12, 8, 8)),
		SimpleCollisionRectangle(Rectangle(8, 8, 16, 16)),
		SimpleCollisionRectangle(Rectangle(4, 4, 24, 24))
	};
}

ExperiencePickup::ExperiencePickup(Graphics &graphics, Units::Game centerX, Units::Game centerY, SizeType size) :
	kinematicsX(centerX - Units::halfTile, 0.025 * (rand() % 11 - 5)),
	kinematicsY(centerY - Units::halfTile, 0.025 * (rand() % 11 - 5)),
	sprite(graphics, spritePath, sourceX, sourceYs[size], sourceW, sourceH, fps, nFrames),
	size(size),
	timer(lifetime, true)
{}

Rectangle ExperiencePickup::collisionRectangle() const{
	return Rectangle(
		kinematicsX.position + collisionRectangles[size].boundingBox().left(),
		kinematicsY.position + collisionRectangles[size].boundingBox().top(),
		collisionRectangles[size].boundingBox().height(),
		collisionRectangles[size].boundingBox().width()
		);
}

bool ExperiencePickup::update(const Units::MS dt, const Map &map){
	updateX(collisionRectangles[size], frictionAccelerator, kinematicsX, kinematicsY, dt, map);
	updateY(collisionRectangles[size], ConstantAccelerator::gravity, kinematicsX, kinematicsY, dt, map, boost::none);

	sprite.update();
	return timer.active();
}

void ExperiencePickup::draw(Graphics &graphics) const{
	if (timer.getCurrentTime() < flickerTime || timer.getCurrentTime() / flickerPeriod % 2 == 0)
		sprite.draw(graphics, kinematicsX.position, kinematicsY.position);
}

int ExperiencePickup::value() const{
	return values[size];
}

void ExperiencePickup::onCollision(sides::SideType side, bool isDeltaDirection, const tiles::TileType tileType){
	if (side == sides::TOP_SIDE)
		kinematicsY.velocity = 0.0;
	else if (side == sides::BOTTOM_SIDE)
		kinematicsY.velocity = -bounceVelocity;
	else 
	 kinematicsX.velocity *= -1;
}