/*
 * canvas.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: nbingham
 */

#include "canvas.h"
#include "../graphics.h"

#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

void canvashdl::initialize()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_current_time		= gtime.tv_sec + gtime.tv_usec*1.0E-6;
	frame_start_time		= gtime.tv_sec + gtime.tv_usec*1.0E-6;
	frame_number			= 0.0;
	game_current_time		= 0.0;
	game_elapsed_time		= 0.0;
	game_time_multiplier	= 1.0;

	player.camera->setup(frustum3f(-1.0, 1.0, -1.0, 1.0, 2.0, 1000.0));
}

void canvashdl::reshape(int w, int h)
{
	screen = vec2i(w, h);
	glViewport(0, 0, w, h);
	player.camera->setup(frustum3f(-(float)w/(float)h, (float)w/(float)h, -1.0, 1.0, 2.0, 1000.0));
}

void canvashdl::release()
{
	screen = vec2i();

	frame_number			= 0.0;
	frame_start_time		= 0.0;
	real_current_time		= 0.0;
	game_current_time		= 0.0;
	game_elapsed_time		= 0.0;
	game_time_multiplier	= 1.0;
}

void canvashdl::render()
{
	handle_time();
	player.handle_controls(game_elapsed_time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	player.camera->projection();
	player.camera->frame();

	glTranslatef(0.0, 0.0, -10.0);
	glutSolidCube(5.0);

	glutSwapBuffers();
}

double canvashdl::handle_time()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_current_time = gtime.tv_sec + gtime.tv_usec*1.0E-6;

	frame_number++;
	game_elapsed_time = (real_current_time - frame_start_time)*game_time_multiplier;
	game_current_time += game_elapsed_time;
	frame_start_time = real_current_time;

	return game_current_time;
}
