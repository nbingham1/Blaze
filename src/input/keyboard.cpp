/*
 * keyboard.cpp
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

#include "keyboard.h"

keyboardhdl::keyboardhdl()
{
	int i;
	for (i = 0; i < 8; i++)
		keystates[i] = 0x00000000;

	type = CNTRL_KEYBOARD;
}

keyboardhdl::~keyboardhdl()
{
	int i;
	for (i = 0; i < 8; i++)
		keystates[i] = 0x00000000;

	type = CNTRL_KEYBOARD;
}

void keyboardhdl::init()
{
	int i;
	for (i = 0; i < 8; i++)
		keystates[i] = 0x00000000;

	type = CNTRL_KEYBOARD;
}

void keyboardhdl::press(unsigned char key)
{
	PRESS_BUTTON(keystates, key);
}

void keyboardhdl::release(unsigned char key)
{
	RELEASE_BUTTON(keystates, key);
}

bool keyboardhdl::get_key(unsigned char key)
{
	return GET_BUTTON(keystates, key);
}

void keyboardhdl::reset()
{
	int i;
	for (i = 0; i < 8; i++)
		keystates[i] = 0x00000000;
}
