/*
 * controller.h
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

#include "../common.h"
#include "../math.h"

#ifndef controller_h
#define controller_h

#include <libusb.h>

// [value][delta][sensitivity]
typedef vec3f		axishdl;

// one bit boolean values
typedef uint8_t		button8hdl[1];
typedef uint16_t	button16hdl[1];
typedef uint32_t	button32hdl[1];
typedef uint32_t	button64hdl[2];
typedef uint32_t	button128hdl[4];
typedef uint32_t	button256hdl[8];

#define PRESS_BUTTON(states, index)		SET_FLAG(states[index/8], FLAG_NUMBER(index%8))
#define RELEASE_BUTTON(states, index)	UNSET_FLAG(states[index/8], FLAG_NUMBER(index%8))
#define GET_BUTTON(states, index)		GET_FLAG(states[index/8], FLAG_NUMBER(index%8))

#define CNTRL_UNKNOWN 0x00

struct controllerhdl
{
	controllerhdl();
	controllerhdl(libusb_device *dev, libusb_config_descriptor *config, int c, int i, int s);
	virtual ~controllerhdl();

	uint8_t 					type;

	int interface_idx;
	int altsetting_idx;
	int configuration_idx;
	libusb_device		 		*device;
	libusb_device_descriptor	device_descriptor;
	libusb_interface			*interface;
	libusb_interface_descriptor *interface_descriptor;
	libusb_config_descriptor	*config_descriptor;

	libusb_device_handle		*handle;

	uint8_t						*report;

	controllerhdl *prev;
	controllerhdl *next;

	virtual void update();

	void *operator new(size_t size, void *v);
};

#endif

