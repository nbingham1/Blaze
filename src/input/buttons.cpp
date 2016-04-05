/*
 * buttons.cpp
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include "buttons.h"
#include "core/file.h"

buttonshdl::buttonshdl()
{

}

buttonshdl::~buttonshdl()
{

}

void buttonshdl::press(int button, double real_current_time, double game_current_time)
{
	array<int>::iterator k = find_first(pressed.ref(), button);
	if (k == pressed.end())
	{
		pressed.push_back(button);

		map<int, preference>::iterator i = control.find(button);
		if (i != control.end())
			i->value(vec3f(1.0f, 1.0f, 0.0f), real_current_time, game_current_time);
	}
}

void buttonshdl::release(int button, double real_current_time, double game_current_time)
{
	array<int>::iterator k = find_first(pressed.ref(), button);
	if (k != pressed.end())
	{
		k.pop();

		map<int, preference>::iterator i = control.find(button);
		if (i != control.end())
			i->value(vec3f(0.0f, -1.0f, 0.0f), real_current_time, game_current_time);
	}
}

bool buttonshdl::get(int button)
{
	return contains(pressed.ref(), button);
}

void buttonshdl::set(int button, bool value, double real_current_time, double game_current_time)
{
	array<int>::iterator k = find_first(pressed.ref(), button);
	if (!value && k != pressed.end())
	{
		k.pop();

		map<int, preference>::iterator i = control.find(button);
		if (i != control.end())
			i->value(vec3f(0.0f, -1.0f, 0.0f), real_current_time, game_current_time);
	}
	else if (value && k == pressed.end())
	{
		pressed.push_back(button);

		map<int, preference>::iterator i = control.find(button);
		if (i != control.end())
			i->value(vec3f(1.0f, 1.0f, 0.0f), real_current_time, game_current_time);
	}
}

void buttonshdl::reset()
{
	pressed.clear();
}
