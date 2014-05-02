#pragma once

#include "util\units.h"

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