/*
 * block.h
 *
 *  Created on: May 23, 2015
 *      Author: nbingham
 */

#include "core/geometry.h"
#include "graphics/opengl.h"

using namespace core;

#ifndef block_h
#define block_h

#define cubes_per_side 64

struct blockhdl
{
	blockhdl();
	blockhdl(vec3F origin, double size, float (*density)(vec3f location, void *data) = NULL);
	~blockhdl();
	void init();

	static array<int> cubes[256];

	array<float> values;
	vec3F origin;
	double size;

	float (*density)(vec3f location, void *data);
	array<vec3f> points;

	array<blockhdl> children;

	inline int index(int i, int j, int k, int width = cubes_per_side+1);
	void generate(void *data = NULL);
	void generate(array<float> svalues, vec3i offset, void *data = NULL);
	void split(void *data = NULL);
	void merge();
	void load();
	void unload();
	void render(int vertex_location, int origin_location);
};

#endif
