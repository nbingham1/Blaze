/*
 * axis.h
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include <math/geometry.h>
#include "preference.h"

using namespace core;

#ifndef axis_h
#define axis_h

struct axishdl
{
	axishdl();
	~axishdl();

	vec3f value;
	float low;
	float high;
	preference control;

	void warp(float v);
	void set(float v, double real_current_time, double game_current_time);
	void move(float v, double real_current_time, double game_current_time);
	void push(float v, double real_current_time, double game_current_time);
	bool contained();

	void reset();
};

#endif
