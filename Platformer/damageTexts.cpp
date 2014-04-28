#include "damageTexts.h"

#include "interfaces\damageable.h"
#include "damageText.h"

void DamageTexts::update(Units::MS dt){
	for (auto&& kv : damageTextMap){
		if (!kv.second.expired()){
			std::shared_ptr<Damageable> damageable(kv.second);
			kv.first->setPosition(damageable->centerX(), damageable->centerY());
		}
		kv.first->update(dt);

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