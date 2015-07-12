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

#include "core/string.h"
#include "core/array.h"
#include "buttons.h"
#include "axis.h"

using namespace core;

#ifndef controller_h
#define controller_h

#define SET_FLAG(res, flag)		(res |= flag)
#define UNSET_FLAG(res, flag)	(res &= ~flag)
#define TOGGLE_FLAG(res, flag)	(res ^= flag)
#define GET_FLAG(res, flag) 	((res & flag) > 0)
#define FLAG_NUMBER(n)			(1 << (n))

struct controllerhdl
{
	controllerhdl();
	controllerhdl(int num_axes);
	~controllerhdl();

	unsigned char flags;

	buttonshdl buttons;
	array<axishdl> axes;

	void update(double real_current_time, double game_current_time);
};

#endif

