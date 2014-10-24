#include "physics.h"

void physicshdl::init(double *rateptr)
{
	mass = 1.0;
	inertia = 1.0;

	radius = 0.0;
	volume = 0.0;
	density = 0.0;

	elasticity = 0.0;

	framerate = rateptr;

	forces = vec();

	position = vec();
	linear_velocity = vec();
	linear_acceleration = vec();
	linear_momentum = vec();

	torques = vec();

	orientation = vec();
	angular_velocity = vec();
	angular_acceleration = vec();
	angular_momentum = vec();
}

void physicshdl::release()
{
	init(framerate);
}

void physicshdl::update()
{
	if (*framerate > 0.0)
	{
		linear_acceleration += forces/mass;
		forces = vec();
		linear_velocity += linear_acceleration/(*framerate);
		linear_acceleration = vec();
		position += linear_velocity/(*framerate);
		linear_momentum = mass*linear_velocity;

		angular_acceleration += torques/inertia;
		torques = vec();
		angular_velocity += angular_acceleration/(*framerate);
		angular_acceleration = vec();
		orientation += angular_velocity/(*framerate);
		angular_momentum = inertia*angular_velocity;
	}
}

void physicshdl::push()
{
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(orientation.x, 1.0, 0.0, 0.0);
	glRotated(orientation.y, 0.0, 1.0, 0.0);
	glRotated(orientation.z, 0.0, 0.0, 1.0);
}

void physicshdl::pop()
{
	glPopMatrix();
}
