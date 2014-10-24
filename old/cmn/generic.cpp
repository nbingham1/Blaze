/*
 * generic.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on December 13, 2011.
 * Modified by Ned Bingham on December 13, 2011.
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

#include "generic.h"
#include <stdlib.h>
#include <stdio.h>

generichdl::generichdl()
{
	super = NULL;
	parent = NULL;
	children_s = NULL;
	children_e = NULL;
	prev = NULL;
	next = NULL;
}

generichdl::~generichdl()
{
	super = NULL;
	parent = NULL;
	children_s = NULL;
	children_e = NULL;
	prev = NULL;
	next = NULL;
}

void generichdl::add_child(generichdl *child)
{
	while (!lock.wrlock());
	printf("adding child!\n");
	if (child == NULL)
		return;

	if (children_s == NULL || children_e == NULL)
	{
		children_s = child;
		children_e = child;
	}
	else
	{
		child->prev = children_e;
		children_e->next = child;
		children_e = child;
	}
	printf("done adding child!\n");
	lock.unlock();
}

void generichdl::rem_child(generichdl *child)
{
	if (child == NULL)
		return;

	while (!lock.wrlock());
	printf("removing child!\n");
	if (children_s == child)
		children_s = children_s->next;
	if (children_e == child)
		children_e = children_e->prev;

	if (child->next)
		child->next->prev = child->prev;
	if (child->prev)
		child->prev->next = child->next;

	child->next = NULL;
	child->prev = NULL;
	printf("done removing child!");
	lock.unlock();
}

void generichdl::render(double t)
{

}

void generichdl::geometry(double t)
{

}

void generichdl::material(double t)
{

}

void generichdl::physics(double t)
{

}

void generichdl::chemistry(double t)
{

}
