#include "accelerators.h"

#include "util\kinematics.h"
#include <algorithm>

const Units::Acceleration gravityAcceleration = 0.00078125;	//fall
const Units::Velocity terminalSpeed = 0.2998046875;

const ZeroAccelerator ZeroAccelerator::zero;

const ConstantAccelerator ConstantAccelerator::gravity(gravityAcceleration, terminalSpeed);

void FrictionAccelerator::updateVelocity(Kinematics &kinematics, Units::MS dt) const{
	kinematics.velocity = kinematics.velocity > 0.f ?
		std::max(0.0, kinematics.velocity - friction*dt) :
		std::min(0.0, kinematics.velocity + friction*dt);
}

void ConstantAccelerator::updateVelocity(Kinematics &kinematics, Units::MS dt) const{
	if (acceleration < 0.0){
		kinematics.velocity = std::max(kinematics.velocity + acceleration * dt, maxVelocity);
	} else {
		kinematics.velocity = std::min(kinematics.velocity + acceleration * dt, maxVelocity);
	}
}