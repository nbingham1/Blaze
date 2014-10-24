/*
 * player.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: nbingham
 */

#include "player.h"

dprefhdl::dprefhdl(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
								  dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8)
{
	controls = d;
	functions[0] = f1;
	functions[1] = f2;
	functions[2] = f3;
	functions[3] = f4;
	functions[4] = f5;
	functions[5] = f6;
	functions[6] = f7;
	functions[7] = f8;
}

dprefhdl::~dprefhdl()
{

}

aprefhdl::aprefhdl(axishdl *a, actrlfn f)
{
	controls = a;
	functions = f;
}

aprefhdl::~aprefhdl()
{

}

playerhdl::playerhdl()
{
	camera = NULL;
}

playerhdl::~playerhdl()
{

}

void playerhdl::handle_controls(double game_elapsed_time)
{
	for (list_node<aprefhdl> *i = analog.begin(); i != analog.end(); i = i->next)
		if (i->functions != NULL)
			(*(i->functions))(game_elapsed_time, this, i->controls);

	for (list_node<dprefhdl> *i = digital.begin(); i != digital.end(); i = i->next)
		for (int j = 0; j < 8; j++)
			if (i->functions[j] != NULL && i->controls != NULL && GET_BUTTON((*(i->controls)), j))
				(*(i->functions[j]))(game_elapsed_time, this);
}

void playerhdl::map_analog(axishdl *a, actrlfn f)
{
	analog.push_back(aprefhdl(a, f));
}

void playerhdl::map_digital(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
										   dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8)
{
	digital.push_back(dprefhdl(d, f1, f2, f3, f4, f5, f6, f7, f8));
}


void forward(double elapsed, playerhdl *player)
{
	player->camera->position += 5.0*-elapsed*ror3(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(player->camera->orientation));
}

void backward(double elapsed, playerhdl *player)
{
	player->camera->position += 5.0*elapsed*ror3(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(player->camera->orientation));
}

void left(double elapsed, playerhdl *player)
{
	player->camera->position += 5.0*-elapsed*ror3(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(player->camera->orientation));
}

void right(double elapsed, playerhdl *player)
{
	player->camera->position += 5.0*elapsed*ror3(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(player->camera->orientation));
}

void up(double elapsed, playerhdl *player)
{
	player->camera->position += 5.0*elapsed*ror3(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(player->camera->orientation));
}

void down(double elapsed, playerhdl *player)
{
	player->camera->position += -5.0*elapsed*ror3(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(player->camera->orientation));
}


void lookh(double elapsed, playerhdl *player, axishdl *data)
{
	player->camera->orientation.data[1] -= data->data[1]*0.1;
	data->data[1] = 0.0;
	data->data[2] = 0.0;
}

void lookv(double elapsed, playerhdl *player, axishdl *data)
{
	player->camera->orientation.data[0] -= data->data[1]*0.1;
	data->data[1] = 0.0;
	data->data[2] = 0.0;
}
