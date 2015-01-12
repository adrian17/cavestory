#pragma once

#include "util/units.h"

struct Kinematics
{
	Kinematics(Units::Game position, Units::Velocity velocity) :
		position(position),
		velocity(velocity)
	{}

	Units::Game position;
	Units::Velocity velocity;

	Units::Game delta(Units::MS dt) {
		return velocity * dt;
	}
};

struct Kinematics2D{
	Position2D position() const {
		return Position2D(x.position, y.position);
	}
	Velocity2D velocity() const {
		return Velocity2D(x.velocity, y.velocity);
	}
	Kinematics x, y;
};