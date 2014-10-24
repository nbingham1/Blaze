/*
 * keyboard.h
 *
 *  Created on: Nov 25, 2013
 *      Author: nbingham
 */

#include "controller.h"

#ifndef keyboard_h
#define keyboard_h

#define CNTRL_KEYBOARD 0x01

struct keyboardhdl : controllerhdl
{
	keyboardhdl();
	~keyboardhdl();

	button256hdl keystates;				// 256 keys

	void init();

	void press(unsigned char key);		// handle key press event
	void release(unsigned char key);	// handle key release event
	bool get_key(unsigned char key);	// get current state of key

	void reset();						// reset all key states to released
};

#endif
