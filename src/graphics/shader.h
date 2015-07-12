/*
 * shader.h
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include "core/string.h"

using namespace core;

#ifndef shader_h
#define shader_h

struct shaderhdl
{
	shaderhdl();
	shaderhdl(unsigned int type, unsigned int identity);
	~shaderhdl();

	unsigned int type;
	unsigned int identity;

	bool load(unsigned int type, string filename);
	bool compile(unsigned int type, string source);
};

bool operator<(shaderhdl s0, shaderhdl s1);
bool operator>(shaderhdl s0, shaderhdl s1);
bool operator<=(shaderhdl s0, shaderhdl s1);
bool operator>=(shaderhdl s0, shaderhdl s1);
bool operator==(shaderhdl s0, shaderhdl s1);
bool operator!=(shaderhdl s0, shaderhdl s1);


#endif
