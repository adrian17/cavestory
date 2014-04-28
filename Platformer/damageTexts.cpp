#include "damageTexts.h"

#include "interfaces\damageable.h"
#include "damageText.h"

void DamageTexts::update(Units::MS dt){
	for (auto iter = damageTextMap.begin(); iter != damageTextMap.end();){
		if (!iter->second.expired()){
			std::shared_ptr<Damageable> damageable(iter->second);
			iter->first->setPosition(damageable->centerX(), damageable->centerY());
		}
		if (iter->first->update(dt) || !iter->second.expired())
			++iter;
		else
			damageTextMap.erase(iter++);

	}
}

void DamageTexts::draw(Graphics &graphics){
	for (auto&& damageText : damageTextMap){
		damageText.first->draw(graphics);
	}
}

void DamageTexts::addDamageable(std::shared_ptr<Damageable> damageable){
	damageTextMap[damageable->getDamageText()] = std::weak_ptr<Damageable>(damageable);
}