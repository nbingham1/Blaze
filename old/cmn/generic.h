/*
 * generic.h
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

#include "lock.h"

#ifndef generic_h
#define generic_h

struct generichdl
{
	generichdl();
	virtual ~generichdl();

	generichdl *super;
	generichdl *parent;
	generichdl *children_s;
	generichdl *children_e;
	generichdl *prev;
	generichdl *next;
	wrlock8_t lock;

	void add_child(generichdl *child);
	void rem_child(generichdl *child);

	virtual void render(double t);
	virtual void geometry(double t);
	virtual void material(double t);
	virtual void physics(double t);
	virtual void chemistry(double t);
};

#endif
