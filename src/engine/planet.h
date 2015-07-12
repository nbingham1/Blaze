/*
 * planet.h
 *
 *  Created on: May 26, 2015
 *      Author: nbingham
 */

#include "core/array.h"
#include "block.h"
#include "object.h"
#include "graphics/palette.h"
#include "core/noise.h"

#ifndef planet_h
#define planet_h

struct cavehdl
{
	cavehdl();
	~cavehdl();

	array<pair<vec3f, float> > control;
	float strength;

	float operator()(vec3f location);
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
	void prepare(palettehdl &palette);
	static float density(vec3f location, void *data);
};

#endif
