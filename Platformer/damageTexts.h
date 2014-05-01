#pragma once

#include "util\units.h"
#include <map>
#include <memory>

class Damageable;
class FloatingNumber;
class Graphics;

class DamageTexts
{
public:
	void update(Units::MS dt);
	void draw(Graphics &graphics);

	void addDamageable(std::shared_ptr<Damageable> damageable);
private:

	typedef std::map<std::shared_ptr<FloatingNumber>, std::weak_ptr<Damageable>> DamageTextMap;
	DamageTextMap damageTextMap;
};

