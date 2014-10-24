/*
 * canvas.h
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
#include "../phys.h"
#include "../mtrl.h"
#include "../input.h"
#include "../gui.h"
#include "../chem.h"

#include "user.h"

#ifndef canvas_h
#define canvas_h

#define CANVAS_VALID 0x01
#define CANVAS_DONE  0x02

void *geometry_canvas(void *data);
void *material_canvas(void *data);
void *physics_canvas(void *data);
void *chemistry_canvas(void *data);
void *input_canvas(void *data);

struct canvashdl : generichdl
{
	canvashdl();
	~canvashdl();

	vec2i screen;

	double frame_number;
	double real_start_time;
	double real_current_time;
	double average_frame_rate;
	double instant_frame_rate;
	double instant_start_frame;
	double instant_start_time;

	double game_current_time;
	double game_time_multiplier;

	char flags;

	userhdl *user_s, *user_e;

	pthread_t threads[4];

	bushdl devices;

	void add_user(userhdl *u);
	void rem_user(userhdl *u);

	void initialize();
	void reshape(int h, int w);
	void release();

	void render();
	void geometry();
	void material();
	void physics();
	void chemistry();

	double handle_time();
};

#endif
