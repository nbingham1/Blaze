/*
 * buttons.cpp
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include "buttons.h"

buttonshdl::buttonshdl()
{

}

buttonshdl::~buttonshdl()
{

}

void buttonshdl::press(int key, double real_current_time, double game_current_time)
{
	array<int>::iterator k = find(pressed, key);
	if (k == pressed.end())
	{
		pressed.push_back(key);

		map<int, preference>::iterator i = control.find(key);
		if (i != control.end())
			i->value(vec3f(1.0f, 1.0f, 0.0f), real_current_time, game_current_time);
	}
}

void buttonshdl::release(int key, double real_current_time, double game_current_time)
{
	array<int>::iterator k = find(pressed, key);
	if (k != pressed.end())
	{
		k.pop();

		map<int, preference>::iterator i = control.find(key);
		if (i != control.end())
			i->value(vec3f(0.0f, -1.0f, 0.0f), real_current_time, game_current_time);
	}
}

bool buttonshdl::get(int key)
{
	return contains(pressed, key);
}

void buttonshdl::reset()
{
	pressed.clear();
}
