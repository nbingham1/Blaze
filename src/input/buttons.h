/*
 * buttons.h
 *
 *  Created on: May 16, 2015
 *      Author: nbingham
 */

#include "core/array.h"
#include "core/map.h"
#include "preference.h"

using namespace core;

#ifndef buttons_h
#define buttons_h

struct buttonshdl
{
	buttonshdl();
	~buttonshdl();

	array<int> pressed;
	map<int, preference> control;

	void press(int button, double real_current_time, double game_current_time);		// handle button press event
	void release(int button, double real_current_time, double game_current_time);	// handle button release event
	bool get(int button);		// get current state of button

	void reset();				// reset all button states to released
};

#endif
