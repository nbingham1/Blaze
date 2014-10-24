/*
 * player.h
 *
 *  Created on: Nov 25, 2013
 *      Author: nbingham
 */

#include "camera.h"
#include "../input.h"

#ifndef player_h
#define player_h

struct playerhdl;

typedef void (*dctrlfn)(double elapsed, playerhdl* player);
typedef void (*actrlfn)(double elapsed, playerhdl* player, axishdl *data);

struct dprefhdl
{
	dprefhdl(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
							dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8);
	~dprefhdl();

	button8hdl *controls;
	dctrlfn functions[8];
};

struct aprefhdl
{
	aprefhdl(axishdl *a, actrlfn f);
	~aprefhdl();

	axishdl *controls;
	actrlfn functions;
};

struct playerhdl
{
	playerhdl();
	~playerhdl();

	camerahdl *camera;

	list<aprefhdl> analog;
	list<dprefhdl> digital;

	void handle_controls(double game_elapsed_time);

	void map_analog(axishdl *a, actrlfn f);
	void map_digital(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
									dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8);
};

void forward(double elapsed, playerhdl *player);
void backward(double elapsed, playerhdl *player);
void left(double elapsed, playerhdl *player);
void right(double elapsed, playerhdl *player);
void up(double elapsed, playerhdl *player);
void down(double elapsed, playerhdl *player);
void lookh(double elapsed, playerhdl *player, axishdl *data);
void lookv(double elapsed, playerhdl *player, axishdl *data);

#endif
