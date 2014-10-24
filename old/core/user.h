/*
 * user.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on May 27, 2012.
 * Modified by Ned Bingham on May 27, 2012.
 * Copyright 2012 Sol Union. All rights reserved.
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

#include "camera.h"
#include "object.h"
#include "../input.h"

#ifndef user_h
#define user_h

struct userhdl;

typedef void (*dctrlfn)(userhdl *);
typedef void (*actrlfn)(userhdl *, axishdl *data);

struct dprefhdl
{
	dprefhdl();
	~dprefhdl();

	button8hdl *controls;
	dctrlfn functions[8];

	dprefhdl *next, *prev;
};

struct aprefhdl
{
	aprefhdl();
	~aprefhdl();

	axishdl *controls;
	actrlfn functions;

	aprefhdl *next, *prev;
};

struct userhdl
{
	userhdl();
	~userhdl();

	camerahdl *cam;
	objecthdl *obj;

	dprefhdl *digital_s, *digital_e;
	aprefhdl *analog_s, *analog_e;

	userhdl *next, *prev;

	void handle_controls();

	void map_analog(axishdl *a, actrlfn f);
	void map_digital(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
									dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8);
};

void forward(userhdl *user);
void backward(userhdl *user);
void left(userhdl *user);
void right(userhdl *user);
void up(userhdl *user);
void down(userhdl *user);
void interact(userhdl *user);
void horizontal(userhdl *user, axishdl *data);
void vertical(userhdl *user, axishdl *data);
void primary(userhdl *user);
void secondary(userhdl *user);
void tertiary(userhdl *user);

#endif
