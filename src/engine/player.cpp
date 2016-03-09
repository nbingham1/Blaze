/*
 * player.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: nbingham
 */

#include "player.h"
#include "graphics/opengl.h"

playerhdl::playerhdl()
{
	camera = NULL;
	speed = 10.0;
	sensitivity = m_pi;
}

playerhdl::~playerhdl()
{
}

void playerhdl::view(framehdl &frame, vec2i screen)
{
	glViewport(screen[0]*viewport[0],
			   screen[1]*viewport[1],
			   screen[0]*viewport[2],
			   screen[1]*viewport[3]);

	float ratio = (float)screen[0]*viewport[2]/((float)screen[1]*viewport[3]);

	if (camera != NULL)
		camera->view(frame, ratio, 2.0, 10000000.0);
}

void playerhdl::forward(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(-player->speed*rotate_zyx(rotate_xyz(vec3d(0.0, 0.0, 1.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(-player->speed*rotate_xyz(vec3d(0.0, 0.0, 1.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::backward(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(player->speed*rotate_zyx(rotate_xyz(vec3d(0.0, 0.0, 1.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(player->speed*rotate_xyz(vec3d(0.0, 0.0, 1.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::left(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(-player->speed*rotate_zyx(rotate_xyz(vec3d(1.0, 0.0, 0.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(-player->speed*rotate_xyz(vec3d(1.0, 0.0, 0.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::right(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(player->speed*rotate_zyx(rotate_xyz(vec3d(1.0, 0.0, 0.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(player->speed*rotate_xyz(vec3d(1.0, 0.0, 0.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::up(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(player->speed*rotate_zyx(rotate_xyz(vec3d(0.0, 1.0, 0.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(player->speed*rotate_xyz(vec3d(0.0, 1.0, 0.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::down(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->impulse(-player->speed*rotate_zyx(rotate_xyz(vec3d(0.0, 1.0, 0.0), player->camera->orientation), player->camera->link->orientation), vec3d(0.0, 0.0, 0.0));
		else
			player->camera->impulse(-player->speed*rotate_xyz(vec3d(0.0, 1.0, 0.0), player->camera->orientation), vec3d(0.0, 0.0, 0.0));
	}
}

void playerhdl::accelerate(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	player->speed *= 1.01;
}

void playerhdl::deccelerate(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	player->speed /= 1.01;
}

void playerhdl::stop(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->velocity = vec3d(0.0, 0.0, 0.0);
		else
			player->camera->velocity = vec3d(0.0, 0.0, 0.0);
	}
}

void playerhdl::horizontal(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->orientation[1] -= player->sensitivity*value[1];
		else
			player->camera->orientation[1] -= player->sensitivity*value[1];
	}
}

void playerhdl::vertical(preference *pref, vec3f value)
{
	playerhdl *player = (playerhdl*)pref->data;
	if (player != NULL && player->camera != NULL)
	{
		if (player->camera->link != NULL)
			player->camera->link->orientation[0] = clamp(player->camera->link->orientation[0] - player->sensitivity*value[1], -m_pi/2.0, m_pi/2.0);
		else
			player->camera->orientation[0] = clamp(player->camera->orientation[0] - player->sensitivity*value[1], -m_pi/2.0, m_pi/2.0);
	}
}

