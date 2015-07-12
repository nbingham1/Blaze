/*
 * axis.cpp
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include "axis.h"

axishdl::axishdl()
{
	low = -INFINITY;
	high = INFINITY;
}

axishdl::~axishdl()
{

}

void axishdl::warp(float v)
{
	value[0] = v;
}

void axishdl::set(float v, double real_current_time, double game_current_time)
{
	value[2] = (v - value[0]) - value[1];
	value[1] = v - value[0];
	value[0] = v;

	if (control.is_valid())
		control(value, real_current_time, game_current_time);
}

void axishdl::move(float v, double real_current_time, double game_current_time)
{
	value[2] = v - value[1];
	value[1] = v;
	value[0] += v;

	if (control.is_valid())
		control(value, real_current_time, game_current_time);
}

void axishdl::push(float v, double real_current_time, double game_current_time)
{
	value[2] = v;
	value[1] += v;
	value[0] += value[1];

	if (control.is_valid())
		control(value, real_current_time, game_current_time);
}

void axishdl::reset()
{
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
}

bool axishdl::contained()
{
	return (value[0] > low && value[0] < high);
}
