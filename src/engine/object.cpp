/*
 * object.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: nbingham
 */

#include "object.h"
#include "canvas.h"

objecthdl::objecthdl()
{
	type = "object";
	selected = false;

	// physics
	last_update_time = 0.0;
	position = vec3F(0, 0, 0);
	velocity = vec3f(0, 0, 0);
	acceleration = vec3f(0, 0, 0);
	mass = 1.0;

	orientation = vec3f(0, 0, 0);
	angular_velocity = vec3f(0, 0, 0);
	angular_acceleration = vec3f(0, 0, 0);
	inertia = 1.0;
}

objecthdl::~objecthdl()
{

}

void objecthdl::impulse(vec3d force, vec3d location)
{
	if (mass != 0.0)
		acceleration += force/mass;

	if (inertia != 0.0)
		angular_acceleration += cross(force, location)/inertia;
}

void objecthdl::init(canvashdl &canvas)
{

}

void objecthdl::prepare(canvashdl &canvas)
{
}

void objecthdl::render(framehdl &frame)
{

}

void objecthdl::clock(double game_current_time)
{
	double elapsed = game_current_time - last_update_time;
	velocity += acceleration*(float)elapsed;
	position += velocity*(float)elapsed;
	angular_velocity += angular_acceleration*(float)elapsed;
	orientation += angular_velocity*(float)elapsed;
	last_update_time = game_current_time;
	acceleration = vec3f(0.0, 0.0, 0.0);
	angular_velocity = vec3f(0.0, 0.0, 0.0);
}

