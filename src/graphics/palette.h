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

	map<string, materialhdl> materials;

	materialhdl *insert(string path);
	void release();
};

#endif
