#include "deathCloudParticle.h"

#include "particle\particleSystem.h"

namespace {
	const std::string spritePath = "npc/NpcSym.bmp";
	const Units::Pixel sourceX = Units::tileToPixel(1);
	const Units::Pixel sourceY = Units::tileToPixel(0);
	const Units::Pixel sourceW = Units::tileToPixel(1);
	const Units::Pixel sourceH = Units::tileToPixel(1);

	const Units::FPS fps = 18;
	const Units::Frame nFrames = 7;

	const Units::Velocity baseVelocity = 0.12;
}

void DeathCloudParticle::createRandomDeathClouds(ParticleTools particleTools,
	Units::Game centerX, Units::Game centerY,
	int nParticles)
{
	for (int i = 0; i < nParticles; ++i){
		particleTools.entitySystem.addNewParticle(std::shared_ptr<Particle>(
			new DeathCloudParticle(particleTools.graphics,
				centerX, centerY,
				(rand() % 3) * baseVelocity,
				rand() % 360)));
	}
}

DeathCloudParticle::DeathCloudParticle(Graphics &graphics,
	Units::Game centerX, Units::Game centerY,
	Units::Velocity velocity, Units::Degrees angle) :
	centerX(centerX - Units::halfTile),
	centerY(centerY - Units::halfTile),
	velocity(velocity),
	offset(0, angle),
	sprite(graphics, spritePath, sourceX, sourceY, sourceW, sourceH, fps, nFrames)
{
}


DeathCloudParticle::~DeathCloudParticle()
{
}

bool DeathCloudParticle::update(const Units::MS dt){
	sprite.update();
	offset.magnitude += velocity * dt;
	return sprite.completedLoops() == 0;
}

void DeathCloudParticle::draw(Graphics &graphics) const{
	sprite.draw(graphics, centerX + offset.getX(), centerY + offset.getY());
}