/*
 * object.h
 *
 *  Created on: Nov 11, 2014
 *      Author: nbingham
 */

#include <math/geometry.h>
#include <std/string.h>
#include <math/big.h>
#include "graphics/frame.h"

using namespace core;

#ifndef object_h
#define object_h

struct canvashdl;

struct objecthdl
{
	objecthdl();
	virtual ~objecthdl();

	// dynamic type checking
	string type;

	// physics
	double last_update_time;
	vec3F position;
	vec3f velocity;
	vec3f acceleration;
	double mass;

	vec3f orientation;
	vec3f angular_velocity;
	vec3f angular_acceleration;
	double inertia;

	// user interface
	bool selected;

	void impulse(vec3d force, vec3d location);

	virtual void init(canvashdl &canvas);
	virtual void prepare(canvashdl &canvas);
	virtual void render(framehdl &frame);
	virtual void clock(double game_current_time);
};


#endif
