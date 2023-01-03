/*
 * canvas.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: nbingham
 */

#include "canvas.h"

#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "block.h"
#include <math/noise.h>

canvashdl::canvashdl()
{
	frame_start_time = 0.0;
	frame_number = 0.0;
	game_time_multiplier = 1.0;
	game_current_time = 0.0;
	real_current_time = 0.0;
}

canvashdl::~canvashdl()
{
	release();
}

void canvashdl::initialize(int w, int h)
{
	// Initialize Time
	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_current_time		= gtime.tv_sec + gtime.tv_usec*1.0E-6;
	frame_number			= 0.0;
	frame_start_time		= real_current_time;
	game_current_time		= 0.0;
	game_time_multiplier	= 1.0;

	// Initialize Display
	screen[0] = w;
	screen[1] = h;

	// Initialize some OpenGL Settings
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0);
	glDisable(GL_CULL_FACE);

	objects.push_back(new planethdl(palette, 0));
}

void canvashdl::reshape(int w, int h)
{
	screen[0] = w;
	screen[1] = h;
}

void canvashdl::release()
{
	players.clear();

	// Clean up the object list
	for (list<objecthdl*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
		if (*obj != NULL)
			delete *obj;
	objects.clear();

	// Clean up the device list
	devices.clear();
}

void canvashdl::prepare()
{
	for (list<objecthdl*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
		if (*obj != NULL)
			(*obj)->prepare(*this);
}

void canvashdl::render()
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i].view(frame, screen);

		vec3F position;
		if (players[i].camera != NULL) {
			position = players[i].camera->position;
		}
		for (list<objecthdl*>::iterator i = objects.begin(); i != objects.end(); i++)
			if ((*i) != NULL && (*i)->type != "camera")
				(*i)->render(frame);
	}
}

void canvashdl::clock()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	real_current_time = gtime.tv_sec + gtime.tv_usec*1.0E-6;

	frame_number++;
	game_current_time += game_time_multiplier*(real_current_time - frame_start_time);
	frame_start_time = real_current_time;

	for (list<objecthdl*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
		if (*obj != NULL)
			(*obj)->clock(game_current_time);
}

void canvashdl::input()
{
	for (map<string, controllerhdl>::iterator i = devices.begin(); i != devices.end(); i++)
		i->value.update(real_current_time, game_current_time);
}

playerhdl *canvashdl::add_player()
{
	playerhdl player;

	float max_area = 0;
	int max_index = -1;
	for (int i = 0; i < players.size(); i++)
	{
		float area = players[i].viewport[2]*players[i].viewport[3];
		if (area > max_area)
		{
			max_area = area;
			max_index = i;
		}
	}

	if (max_index == -1)
		player.viewport = vec4f(0.0, 0.0, 1.0, 1.0);
	else
	{
		player.viewport = players[max_index].viewport;
		if (player.viewport[2] > player.viewport[3])
		{
			player.viewport[2] /= 2.0;
			player.viewport[0] += player.viewport[2];
			players[max_index].viewport[2] /= 2.0;
		}
		else
		{
			player.viewport[3] /= 2.0;
			player.viewport[1] += player.viewport[3];
			players[max_index].viewport[3] /= 2.0;
		}
	}

	camerahdl *camera = new camerahdl();
	objects.push_back(camera);
	player.camera = camera;
	players.push_back(player);

	return &players.back();
}
