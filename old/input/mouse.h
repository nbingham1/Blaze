/*
 * mouse.h
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
#include "../math.h"

#ifndef mouse_h
#define mouse_h

#define CNTRL_MOUSE 0x02

#define MOUSE_CLEAR			0x00

#define MOUSE_BIND			0x01
#define MOUSE_WARP			0x02

#define MOUSE_PRIMARY		0x01
#define MOUSE_SECONDARY 	0x02
#define MOUSE_TERTIARY		0x04
#define MOUSE_QUATERNARY	0x08
#define MOUSE_QUINTINARY	0x10
#define MOUSE_SEXTIARY		0x20
#define MOUSE_SEPTIMARY		0x40
#define MOUSE_OCTAVARY		0x80

struct mousehdl : controllerhdl
{
	mousehdl();
	mousehdl(libusb_device *dev, libusb_config_descriptor *config, int c, int i, int s);
	virtual ~mousehdl();

	vec<axishdl, 2>	pointer;						// [vertical, horizontal]
	button8hdl		buttons;

	uint8_t			flags;							// [][][][][][][][bind]

	void init();

	void hide();
	void show();
	void toggle();									// Show/Hide mouse

	void press(int button);							// Handle mouse press event
	void release(int button);						// Handle mouse release event

	void update();
};

#endif
