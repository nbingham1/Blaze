/*
 * user.cpp
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

#include "user.h"
#include "../common.h"
#include "../math.h"

dprefhdl::dprefhdl()
{
	controls = NULL;
	for (int i = 0; i < 8; i++)
		functions[i] = NULL;

	next = NULL;
	prev = NULL;
}

dprefhdl::~dprefhdl()
{
	controls = NULL;
	for (int i = 0; i < 8; i++)
		functions[i] = NULL;

	next = NULL;
	prev = NULL;
}

aprefhdl::aprefhdl()
{
	for (int i = 0; i < 2; i++)
	{
		controls = NULL;
		functions = NULL;
	}

	next = NULL;
	prev = NULL;
}

aprefhdl::~aprefhdl()
{
	for (int i = 0; i < 2; i++)
	{
		controls = NULL;
		functions = NULL;
	}

	next = NULL;
	prev = NULL;
}

userhdl::userhdl()
{
	cam = NULL;
	obj = NULL;
	next = NULL;
	prev = NULL;

	digital_s = NULL;
	digital_e = NULL;
	analog_s = NULL;
	analog_e = NULL;
}

userhdl::~userhdl()
{
	dprefhdl *currd = digital_s, *prevd = NULL;
	while (currd != NULL)
	{
		prevd = currd;
		currd = currd->next;
		delete prevd;
		prevd = NULL;
	}

	aprefhdl *curra = analog_s, *preva = NULL;
	while (curra != NULL)
	{
		preva = curra;
		curra = curra->next;
		delete preva;
		preva = NULL;
	}

	cam = NULL;
	obj = NULL;
	next = NULL;
	prev = NULL;

	digital_s = NULL;
	digital_e = NULL;
	analog_s = NULL;
	analog_e = NULL;
}

void userhdl::handle_controls()
{
	unsigned char i = 0;

	dprefhdl *currd = digital_s;
	while (currd != NULL)
	{
		for (i = 0; i < 8; i++)
			if (currd->functions[i] != NULL && GET_BUTTON(*(currd->controls), i))
				currd->functions[i](this);
		currd = currd->next;
	}

	aprefhdl *curra = analog_s;
	while (curra != NULL)
	{
		if (curra->functions != NULL)
			curra->functions(this, curra->controls);
		curra = curra->next;
	}
}

void userhdl::map_analog(axishdl *a, actrlfn f)
{
	aprefhdl *pref = new aprefhdl();
	pref->controls = a;
	pref->functions = f;

	if (analog_s == NULL || analog_e == NULL)
	{
		analog_s = pref;
		analog_e = pref;
	}
	else
	{
		analog_e->next = pref;
		pref->prev = analog_e;
		analog_e = analog_e->next;
	}
}

void userhdl::map_digital(button8hdl *d, dctrlfn f1, dctrlfn f2, dctrlfn f3, dctrlfn f4,
										 dctrlfn f5, dctrlfn f6, dctrlfn f7, dctrlfn f8)
{
	dprefhdl *pref = new dprefhdl();
	pref->controls = d;
	pref->functions[0] = f1;
	pref->functions[1] = f2;
	pref->functions[2] = f3;
	pref->functions[3] = f4;
	pref->functions[4] = f5;
	pref->functions[5] = f6;
	pref->functions[6] = f7;
	pref->functions[7] = f8;

	if (digital_s == NULL || digital_e == NULL)
	{
		digital_s = pref;
		digital_e = pref;
	}
	else
	{
		digital_e->next = pref;
		pref->prev = digital_e;
		digital_e = digital_e->next;
	}
}

void forward(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(-0.0001*rol(ror(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(-0.0001*ror(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void backward(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(0.0001*rol(ror(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(0.0001*ror(vec4f(0.0, 0.0, 1.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void left(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(-0.0001*rol(ror(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(-0.0001*ror(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void right(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(0.0001*rol(ror(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(0.0001*ror(vec4f(1.0, 0.0, 0.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void up(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(0.0001*rol(ror(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(0.0001*ror(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void down(userhdl *user)
{
	if (user != NULL && user->cam != NULL)
	{
		if (user->obj != NULL)
			user->obj->force_field(-0.0001*rol(ror(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(user->cam->orientation())), degtorad(user->obj->orientation())));
		else
			user->cam->force_field(-0.0001*ror(vec4f(0.0, 1.0, 0.0, 0.0), degtorad(user->cam->orientation())));
	}
}

void interact(userhdl *user)
{

}

void horizontal(userhdl *user, axishdl *data)
{
	if (user == NULL || user->cam == NULL)
		return;

	//user->cam->torque(vec4f(-(*data)[2], 0.0, 0.0, 0.0));
	data->print();
}

void vertical(userhdl *user, axishdl *data)
{
	//if (data != NULL)
	//	data->print();
}

void primary(userhdl *user)
{

}

void secondary(userhdl *user)
{

}

void tertiary(userhdl *user)
{

}
