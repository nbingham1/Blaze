/*
 * canvas.cpp
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
#include "canvas.h"

void *geometry_canvas(void *data)
{
	canvashdl *canvas = (canvashdl*)data;

	while (!GET_FLAG(canvas->flags, CANVAS_DONE))
		canvas->geometry();

	return NULL;
}

void *material_canvas(void *data)
{
	canvashdl *canvas = (canvashdl*)data;

	while (!GET_FLAG(canvas->flags, CANVAS_DONE))
		canvas->material();

	return NULL;
}

void *physics_canvas(void *data)
{
	canvashdl *canvas = (canvashdl*)data;

	while (!GET_FLAG(canvas->flags, CANVAS_DONE))
		canvas->physics();

	return NULL;
}

void *chemistry_canvas(void *data)
{
	canvashdl *canvas = (canvashdl*)data;

	while (!GET_FLAG(canvas->flags, CANVAS_DONE))
		canvas->chemistry();

	return NULL;
}

canvashdl::canvashdl()
{
	screen = vec2i();

	frame_number		= 0.0;
	real_start_time		= 0.0;
	real_current_time	= 0.0;
	average_frame_rate	= 0.0;
	instant_frame_rate	= 0.0;
	instant_start_frame	= 0.0;
	instant_start_time	= 0.0;

	game_current_time		= 0.0;
	game_time_multiplier	= 1.0;

	flags = 0x00;

	user_s = NULL;
	user_e = NULL;
}

canvashdl::~canvashdl()
{
	release();
}

void canvashdl::add_user(userhdl *u)
{
	if (user_s == NULL || user_e == NULL)
	{
		user_s = u;
		user_e = u;
	}
	else
	{
		user_e->next = u;
		u->prev = user_e;
		user_e = user_e->next;
	}
}

void canvashdl::rem_user(userhdl *u)
{
	if (user_s == u)
		user_s = u->next;
	if (user_e == u)
		user_e = u->prev;

	if (u->next)
		u->next->prev = u->prev;
	if (u->prev)
		u->prev->next = u->next;

	u->next = NULL;
	u->prev = NULL;
}

void canvashdl::initialize()
{
	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_start_time		= gtime.tv_sec + gtime.tv_usec*1.0E-6;
	real_current_time	= real_start_time;
	instant_start_time	= real_start_time;

	devices.init();

	UNSET_FLAG(flags, CANVAS_DONE);
	pthread_create(&threads[0], NULL, geometry_canvas, this);
	pthread_create(&threads[1], NULL, material_canvas, this);
	pthread_create(&threads[2], NULL, physics_canvas, this);
	pthread_create(&threads[3], NULL, chemistry_canvas, this);
	SET_FLAG(flags, CANVAS_VALID);
}

void canvashdl::reshape(int w, int h)
{
	screen = vec2i(w, h);
	glViewport(0, 0, w, h);
}

void canvashdl::release()
{
	int i;

	if (GET_FLAG(flags, CANVAS_VALID))
	{
		SET_FLAG(flags, CANVAS_DONE);
		for (i = 0; i < 4; i++)
			pthread_join(threads[i], NULL);

		userhdl *curr_user = user_s, *prev_user;
		while (curr_user != NULL)
		{
			prev_user = curr_user;
			curr_user = curr_user->next;
			delete prev_user;
			prev_user = NULL;
		}

		screen = vec2i();

		frame_number		= 0.0;
		real_start_time		= 0.0;
		real_current_time	= 0.0;
		average_frame_rate	= 0.0;
		instant_frame_rate	= 0.0;
		instant_start_frame	= 0.0;
		instant_start_time	= 0.0;

		game_current_time		= 0.0;
		game_time_multiplier	= 1.0;

		UNSET_FLAG(flags, CANVAS_VALID);
		SET_FLAG(flags, CANVAS_DONE);

		user_s = NULL;
		user_e = NULL;

		devices.release();
	}
}

void canvashdl::render()
{
	handle_time();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (user_s != NULL)
	{
		//user_s->handle_controls();
		user_s->cam->projection();
		user_s->cam->frame();
	}

	glTranslatef(0.0, 0.0, -10.0);
	glutSolidCube(5.0);

	/*generichdl *curr = children_s;
	while (curr != NULL)
	{
		curr->render(game_current_time);
		curr = curr->next;
	}*/

	glutSwapBuffers();
}

void canvashdl::geometry()
{
	while (!lock.rdlock());
	generichdl *curr = children_s;
	while (curr != NULL)
	{
		curr->geometry(game_current_time);
		curr = curr->next;
	}
	lock.unlock();
}

void canvashdl::material()
{
	while (!lock.rdlock());
	generichdl *curr = children_s;
	while (curr != NULL)
	{
		curr->material(game_current_time);
		curr = curr->next;
	}
	lock.unlock();
}

void canvashdl::physics()
{
	while (!lock.rdlock());
	printf("physics!\n");
	dualityhdl *i1 = (dualityhdl*)children_s;
	while (i1 != NULL)
	{
		//i1->physics(game_current_time);
		i1 = (dualityhdl*)i1->next;
	}
	printf("physics done!\n");
	lock.unlock();
}

void canvashdl::chemistry()
{
	while (!lock.rdlock());
	generichdl *curr = children_s;
	while (curr != NULL)
	{
		curr->chemistry(game_current_time);
		curr = curr->next;
	}
	lock.unlock();
}

double canvashdl::handle_time()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_current_time = gtime.tv_sec + gtime.tv_usec*1.0E-6;

	frame_number++;
	average_frame_rate = frame_number/(real_current_time - real_start_time);

	if (frame_number - instant_start_frame >= 5)
	{
		instant_frame_rate = (frame_number - instant_start_frame)/(real_current_time - instant_start_time);
		instant_start_frame = frame_number;
		instant_start_time = real_current_time;
	}

	game_current_time = (real_current_time - real_start_time)*game_time_multiplier;

	return game_current_time;
}
