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

#include "common.h"
#include "../base.h"
#include "../math.h"
#include <stdint.h>

#ifndef controller_h
#define controller_h

#define SET_FLAG(res, flag)		(res |= flag)
#define UNSET_FLAG(res, flag)	(res &= ~flag)
#define TOGGLE_FLAG(res, flag)	(res ^= flag)
#define GET_FLAG(res, flag) 	((res & flag) > 0)
#define FLAG_NUMBER(n)			(1 << (n))

// [value][delta][sensitivity]
typedef vec3f		axishdl;

// one bit boolean values
typedef uint8_t		button8hdl[1];
typedef uint16_t	button16hdl[1];
typedef uint32_t	button32hdl[1];
typedef uint32_t	button64hdl[2];
typedef uint32_t	button128hdl[4];
typedef uint32_t	button256hdl[8];

#define PRESS_BUTTON(states, index)		SET_FLAG(states[index/32], FLAG_NUMBER(index%32))
#define RELEASE_BUTTON(states, index)	UNSET_FLAG(states[index/32], FLAG_NUMBER(index%32))
#define GET_BUTTON(states, index)		GET_FLAG(states[index/32], FLAG_NUMBER(index%32))

#define CNTRL_UNKNOWN 0x00

struct controllerhdl
{
	controllerhdl();
	virtual ~controllerhdl();

	uint8_t type;
};

#endif

