/*
 * ether.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 13, 2012.
 * Modified by Ned Bingham on June 13, 2012.
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

#include "ether.h"

mat<float, 8, 3> corners(-0.5, -0.5, -0.5,
						  0.5, -0.5, -0.5,
						 -0.5,  0.5, -0.5,
						  0.5,  0.5, -0.5,
						 -0.5, -0.5,  0.5,
						  0.5, -0.5,  0.5,
						 -0.5,  0.5,  0.5,
						  0.5,  0.5,  0.5);

etheraddr::etheraddr()
{
	group = NULL;
	index = 0;
}

etheraddr::etheraddr(ethergrp *grp, char idx)
{
	group = grp;
	index = idx;
}

etheraddr::~etheraddr()
{
	group = NULL;
	index = 0;
}

etheraddr &etheraddr::operator=(etheraddr address)
{
	group = address.group;
	index = address.index;
	return *this;
}

etherprt::etherprt()
{
	size		= 0.0f;

	position	= vec3f();
	momentum	= vec3f();
	force		= vec3f();

	children	= NULL;
}

etherprt::~etherprt()
{
	size		= 0.0f;

	position	= vec3f();
	momentum	= vec3f();
	force		= vec3f();

	children	= NULL;
}

ethergrp::ethergrp()
{
	clear();
	super = NULL;
	next  = NULL;
}

ethergrp::ethergrp(etheraddr address)
{
	clear();
	super  = NULL;
	next   = NULL;
	parent = address;
}

ethergrp::~ethergrp()
{
	clear();
	super = NULL;
	next  = NULL;
}

etheraddr ethergrp::classify_down(vec3f object)
{
	unsigned char idx = (object[0] >= divider[0]) | ((object[1] >= divider[1]) << 1) | ((object[2] >= divider[2]) << 2);
	return (parts[idx].children != NULL ? parts[idx].children->classify_down(object) : etheraddr(this, idx));
}

etheraddr ethergrp::classify(vec3f object)
{
	if (parent.group == NULL)
		return etheraddr();
	else if (object <= corners[7]*parent.group->parts[(int)parent.index].size && object >= corners[0]*parent.group->parts[(int)parent.index].size)
		return classify_down(object);
	else
		return parent.group->classify(object + parent.group->parts[(int)parent.index].position);
}

void ethergrp::subdivide(char idx)
{
	if (parts[(int)idx].children != NULL)
		for (int i = 0; i < 8; i++)
			parts[(int)idx].children->subdivide(i);
	else
	{
		ethergrp *children = super->allocate();
		children->divider = parts[(int)idx].position;

		for (int i = 0; i < 8; i++)
		{
			children->parts[i].size		= parts[(int)idx].size*0.5;
			children->parts[i].position	= corners[i]*children->parts[i].size;
			children->parts[i].momentum	= parts[(int)idx].momentum*0.125;
			children->parts[i].force	= parts[(int)idx].force;
			children->parts[i].children	= NULL;
		}

		parts[(int)idx].children = children;
	}
}

void ethergrp::superdivide(char idx)
{
	if (parent.group != NULL)
		parent.group->superdivide(idx);
	else
	{
		parent.group = super->allocate();
		parent.index = idx;
		parent.group->clear();
		parent.group->divider = divider;

		parent.group->parts[(int)idx].children = this;
		parent.group->parts[(int)idx].force = 0;
		parent.group->parts[(int)idx].momentum = vec3f();
		parent.group->parts[(int)idx].size = parts[0].size*2.0;
		parent.group->parts[(int)idx].position = divider;
	}
}

bool ethergrp::submerge(char idx)
{
	if (parts[(int)idx].children != NULL)
	{
		bool grandchildren = false;
		for (int i = 0; i < 8; i++)
			grandchildren |= parts[(int)idx].children->submerge(i);

		if (!grandchildren)
		{
			super->deallocate(&parts[(int)idx].children);
			return false;
		}

		return true;
	}

	return false;
}

void ethergrp::supermerge()
{
	if (parent.group != NULL)
	{
		if (parent.group->parent.group == NULL)
		{
			for (int i = 0; i < 8; i++)
				if (parent.group->parts[i].children != this)
					parent.group->subtrim(i);

			super->deallocate(&parent.group);
			super->top = this;
		}
		else
			parent.group->supermerge();
	}
}

void ethergrp::subtrim(char idx)
{
	if (parts[(int)idx].children != NULL)
	{
		for (int i = 0; i < 8; i++)
			parts[(int)idx].children->subtrim(i);

		super->deallocate(&parts[(int)idx].children);
	}
}

void ethergrp::supertrim()
{
	if (parent.group != NULL)
	{
		parent.group->supertrim();

		for (int i = 0; i < 8; i++)
			if (parent.group->parts[i].children != this)
				parent.group->subtrim(i);

		super->deallocate(&parent.group);
		super->top = this;
	}
}

void ethergrp::clear()
{
	for (int i = 0; i < 8; i++)
	{
		parts[i].children = NULL;
		parts[i].force = vec3f();
		parts[i].momentum = vec3f();
		parts[i].size = 0.0f;
		parts[i].position = vec3f();
	}

	this->divider = vec3f();
}

etherhdl::etherhdl()
{
	unused = buffer;
	for (int i = 1; i < ETHER_NUM_GROUPS; i++)
	{
		if (i < ETHER_NUM_GROUPS-1)
			buffer[i].next = &(buffer[i+1]);
		buffer[i].super = this;
	}

	top = allocate();
}

etherhdl::~etherhdl()
{

}

ethergrp *etherhdl::allocate()
{
	if (unused == NULL)
		return NULL;

	ethergrp *result = unused;
	unused = result->next;
	result->next = NULL;
	return result;
}

void etherhdl::deallocate(ethergrp **prt)
{
	(*prt)->next = unused;
	unused = *prt;
	*prt = NULL;
}
