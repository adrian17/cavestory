#include "timer.h"

std::set<Timer*> Timer::timers;

void Timer::updateAll(Units::MS dt){
	for (auto &&timer : timers){
		timer->update(dt);
	}
}