/*
 * planet.h
 *
 *  Created on: May 26, 2015
 *      Author: nbingham
 */

#include <std/array.h>
#include "block.h"
#include "object.h"
#include "graphics/palette.h"
#include <math/noise.h>

#ifndef planet_h
#define planet_h

struct cavehdl
{
	cavehdl();
	~cavehdl();

	array<pair<vec3f, float> > control;
	float strength;

	grad3f operator()(gvec3f location);
};

struct planethdl : objecthdl
{
	planethdl();
	planethdl(palettehdl &palette, int seed);
	~planethdl();

	blockhdl geometry;
	noise3hdl noise;
	array<cavehdl> caves;
	array<cavehdl> spires;

	int program;

	void render(framehdl &frame);
	void prepare(canvashdl &canvas);
	static grad3f density(gvec3f location, void *data);
};

#endif
