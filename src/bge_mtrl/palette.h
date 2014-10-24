/*
 * palette.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "material.h"

#ifndef palette_h
#define palette_h

struct palettehdl
{
	palettehdl();
	~palettehdl();

	materialhdl *palette_s, *palette_e;

	materialhdl *add(const char *directory);
	void add(materialhdl **m);
	void remove(materialhdl *m);
	void release();
};

#endif
