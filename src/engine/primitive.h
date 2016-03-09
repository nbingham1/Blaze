#pragma once

#include "graphics/opengl.h"
#include "rigid.h"

struct boxhdl : rigidhdl
{
	boxhdl(float width, float height, float depth);
	~boxhdl();
};

struct spherehdl : rigidhdl
{
	spherehdl(float radius, int levels, int slices);
	~spherehdl();
};

struct cylinderhdl : rigidhdl
{
	cylinderhdl(float radius, float height, int slices);
	~cylinderhdl();
};

struct pyramidhdl : rigidhdl
{
	pyramidhdl(float radius, float height, int slices);
	~pyramidhdl();
};
