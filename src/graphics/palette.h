/*
 * palette.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "core/map.h"
#include "shader.h"
#include "texture.h"

using namespace core;

#ifndef palette_h
#define palette_h

struct palettehdl
{
	palettehdl();
	~palettehdl();

	map<string, texturehdl> textures;
	map<string, shaderhdl> shaders;
	map<array<shaderhdl>, unsigned int> programs;

	unsigned int program(string vertex, string fragment);
	unsigned int program(string vertex, string geometry, string fragment);
	texturehdl texture(unsigned int type, string filename);
};

#endif
