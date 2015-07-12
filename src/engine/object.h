/*
 * object.h
 *
 *  Created on: Nov 11, 2014
 *      Author: nbingham
 */

#include "core/geometry.h"
#include "core/string.h"
#include "core/big.h"
#include "graphics/frame.h"
#include "graphics/palette.h"

using namespace core;

#ifndef object_h
#define object_h

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

	virtual void init(palettehdl &palette);
	virtual void prepare(palettehdl &palette);
	virtual void render(framehdl &frame);
	virtual void clock(double game_current_time);
};


#endif
