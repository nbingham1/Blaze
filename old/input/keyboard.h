/*
 * keyboard.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 7, 2011.
 * Modified by Ned Bingham on December 7, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller.h"

#ifndef keyboard_h
#define keyboard_h

#define CNTRL_KEYBOARD 0x01

struct keyboardhdl : controllerhdl
{
	keyboardhdl();
	keyboardhdl(libusb_device *dev, libusb_config_descriptor *config, int c, int i, int s);
	~keyboardhdl();
	
	button256hdl keystates;				// 256 keys
	
	void init();

	void press(unsigned char key);		// handle key press event
	void release(unsigned char key);	// handle key release event
	bool get_key(unsigned char key);	// get current state of key

	void reset();						// reset all key states to released

	void update();
};

#endif
