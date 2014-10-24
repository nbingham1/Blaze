/*
 * mouse.cpp
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

#include "mouse.h"
#include "../common.h"

mousehdl::mousehdl()
{
	flags	= MOUSE_CLEAR;
	buttons[0] = MOUSE_CLEAR;
	type = CNTRL_MOUSE;
}

mousehdl::mousehdl(libusb_device *dev, libusb_config_descriptor *config, int c, int i, int s) : controllerhdl(dev, config, c, i, s)
{
	flags	= MOUSE_CLEAR;
	buttons[0] = MOUSE_CLEAR;
	type = CNTRL_MOUSE;
}

mousehdl::~mousehdl()
{
	flags	= MOUSE_CLEAR;
	buttons[0] = MOUSE_CLEAR;
	type = CNTRL_MOUSE;
}

void mousehdl::init()
{
	flags	= MOUSE_CLEAR;
	buttons[0] = MOUSE_CLEAR;
	type = CNTRL_MOUSE;
}

void mousehdl::hide()
{
	SET_FLAG(flags, MOUSE_BIND);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void mousehdl::show()
{
	UNSET_FLAG(flags, MOUSE_BIND);
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
}

void mousehdl::toggle()
{
	TOGGLE_FLAG(flags, MOUSE_BIND);
	if (GET_FLAG(flags, MOUSE_BIND))
		glutSetCursor(GLUT_CURSOR_NONE);
	else
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
}

void mousehdl::press(int button)
{
	PRESS_BUTTON(buttons, button);
}

void mousehdl::release(int button)
{
	RELEASE_BUTTON(buttons, FLAG_NUMBER(button));
}

void mousehdl::update()
{
	// TODO
	/*const libusb_endpoint_descriptor *endpoint = &interface_descriptor->endpoint[0];
	unsigned char buffer[endpoint->wMaxPacketSize];
	int length = 0;

	libusb_interrupt_transfer(handle, endpoint->bEndpointAddress, buffer, endpoint->wMaxPacketSize, &length, 5000);*/
}

