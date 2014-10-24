/*
 * material.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "texture.h"
#include "shader.h"

#ifndef material_h
#define material_h

struct materialhdl
{
	materialhdl();
	materialhdl(const materialhdl &m);
	materialhdl(string path);
	~materialhdl();

	texturehdl texture;
	shaderhdl shader;

	void release();
	void bind();

	materialhdl &operator=(materialhdl m);
};

#endif
