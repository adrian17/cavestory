#pragma once

#include "util\units.h"
#include <map>
#include <memory>

class Damageable;
class DamageText;
class Graphics;

class DamageTexts
{
public:
	void update(Units::MS dt);
	void draw(Graphics &graphics);

	void addDamageable(std::shared_ptr<Damageable> damageable);
private:

	typedef std::map<std::shared_ptr<DamageText>, std::weak_ptr<Damageable>> DamageTextMap;
	DamageTextMap damageTextMap;
};

