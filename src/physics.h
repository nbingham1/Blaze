#include "vector.h"
#include "mathematics.h"
#include "standard.h"
#include "graphics.h"

#ifndef physics_h
#define physics_h

//G = 6.67300*10^-11 m^3 kg^-1 s^-2

struct physicshdl
{
	double mass;
	double inertia;

	double radius;
	double volume;
	double density;

	double elasticity;

	double *framerate;

	vec forces;

	vec position;
	vec linear_velocity;
	vec linear_acceleration;
	vec linear_momentum;

	vec torques;

	vec orientation;
	vec angular_velocity;
	vec angular_acceleration;
	vec angular_momentum;

	void init(double *rateptr);
	void release();

	void update();

	void push();
	void pop();
};

#endif
