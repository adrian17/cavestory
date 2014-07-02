#pragma once

#include "util\units.h"
#include <set>

class Timer
{
public:
	Timer(Units::MS expirationTime, bool startNow = false) :
		currentTime(startNow ? 0 : expirationTime),
		expirationTime(expirationTime) 
	{
		timers.insert(this);
	}

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	Timer::~Timer(){ timers.erase(this); }

	Units::MS getCurrentTime() const { return currentTime; }
	bool active() const { return currentTime < expirationTime; }
	bool expired() const { return !active(); }
	void reset() { currentTime = 0; }

	static void updateAll(Units::MS dt);
private:
	void update(const Units::MS dt){
		if (active())
			currentTime += dt;
	}

	Units::MS currentTime;
	const Units::MS expirationTime;

	static std::set<Timer*> timers;
};