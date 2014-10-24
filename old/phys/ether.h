/*
 * ether.h
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

#include "../common.h"
#include "../math.h"

#ifndef ether_h
#define ether_h

// This is enough levels to get from 10^-35 meters to
// 10^27 meters which is more range than real life has.
#define ETHER_MAX_SPLITS 206
// If we allowed the max number of splits uniformly
// throughout the universe, we would need
// 8^206 + 8^205 + ... + 8^1 + 8^0 etherprts, which
// would take up more memory than exists in the world.
// We can get away with fewer.
#define ETHER_LEVEL_MIN_SPLITS 10
#define ETHER_NUM_GROUPS (ETHER_MAX_SPLITS*ETHER_LEVEL_MIN_SPLITS)

struct etherhdl;
struct ethergrp;

// 5 bytes
struct etheraddr
{
	etheraddr();
	etheraddr(ethergrp *grp, char idx);
	~etheraddr();

	ethergrp *group;
	char	  index;

	etheraddr &operator=(etheraddr address);
};

// 64 bytes
struct etherprt
{
	etherprt();
	~etherprt();

	float size;

	vec3f position;
	vec3f momentum;
	vec3f force;

	ethergrp *children;
};

// 525 bytes
struct ethergrp
{
	ethergrp();
	ethergrp(etheraddr address);
	~ethergrp();

	etherprt  parts[8];
	vec3f	  divider;

	etherhdl  *super;
	etheraddr parent;
	ethergrp  *next;

	etheraddr classify_down(vec3f object);
	etheraddr classify(vec3f object);

	void subdivide(char idx);
	void superdivide(char idx);
	bool submerge(char idx);
	void supermerge();
	void subtrim(char idx);
	void supertrim();

	void clear();
};

struct etherhdl
{
	etherhdl();
	~etherhdl();

	ethergrp buffer[ETHER_NUM_GROUPS];
	ethergrp *unused;

	ethergrp *top;

	ethergrp *allocate();
	void deallocate(ethergrp **grp);
};

#endif
