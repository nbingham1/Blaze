#pragma once

#include "graphics/opengl.h"
#include <std/array.h>
#include <math/geometry.h>

using namespace core;

struct rigidhdl
{
	rigidhdl();
	~rigidhdl();

	array<vec8f> geometry;
	array<int> indices;

	void render(int vertex_location, int normal_location = -1, int texcoord_location = -1);
};
