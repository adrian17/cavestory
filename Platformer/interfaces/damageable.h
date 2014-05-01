#pragma once

#include "util\units.h"
#include <memory>

class FloatingNumber;

class Damageable{
public:
	virtual Units::Game centerX() const = 0;
	virtual Units::Game centerY() const = 0;
	virtual std::shared_ptr<FloatingNumber> getDamageText() = 0;

	virtual ~Damageable() = 0;
};

inline Damageable::~Damageable(){}