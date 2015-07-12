/*
 * material.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "texture.h"

#include "core/string.h"
#include "core/map.h"

using namespace core;

#ifndef material_h
#define material_h

struct materialhdl
{
	materialhdl();
	~materialhdl();

	unsigned int program;
	map<string, texturehdl> textures;
	map<string, array<float> > values;

	void bind();
};

#endif
