#pragma once

#include "util\units.h"

struct PolarVector{
	PolarVector(Units::Game magnitude, Units::Degrees angle) :
		magnitude(magnitude), angle(angle){}
	Units::Game magnitude;
	Units::Degrees angle;

	Units::Game getX() const{
		return magnitude * Units::cos(angle);
	}
	Units::Game getY() const{
		return magnitude * Units::sin(angle);
	}

};