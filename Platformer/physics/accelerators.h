#pragma once

#include "util/units.h"

struct Kinematics;

extern const Units::Velocity terminalSpeed;

struct Accelerator{
	Accelerator(){}
	virtual ~Accelerator() = 0;
	virtual void updateVelocity(Kinematics &kinematics, Units::MS dt) const = 0;
};
inline Accelerator::~Accelerator(){}

struct ZeroAccelerator : Accelerator{
	ZeroAccelerator(){}
	void updateVelocity(Kinematics&, Units::MS) const{}
	static const ZeroAccelerator zero;
};

struct FrictionAccelerator : Accelerator{
	FrictionAccelerator(Units::Acceleration friction) :
		friction(friction)
	{}
	void updateVelocity(Kinematics &kinematics, Units::MS dt) const;
private:
	Units::Acceleration friction;
};

struct ConstantAccelerator : Accelerator{
	ConstantAccelerator(Units::Acceleration acceleration, Units::Velocity maxVelocity) :
		acceleration(acceleration), maxVelocity(maxVelocity)
	{}
	void updateVelocity(Kinematics &kinematics, Units::MS dt) const;
	static const ConstantAccelerator gravity;
private:
	Units::Acceleration acceleration;
	Units::Velocity maxVelocity;
};

struct BidirectionalAccelerators{
	BidirectionalAccelerators(Units::Acceleration acceleration, Units::Velocity maxVelocity) :
		positive(acceleration, maxVelocity),
		negative(-acceleration, -maxVelocity)
	{}
	ConstantAccelerator positive, negative;
};