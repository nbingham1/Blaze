/*
 * texture.h
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include "image.h"

#ifndef texture_h
#define texture_h

struct texturehdl
{
	texturehdl();
	texturehdl(unsigned int type, unsigned int identity);
	~texturehdl();

	unsigned int type;
	unsigned int identity;

	bool load(unsigned int type, const imagehdl &image);

	void bind();
	void release();

	void frame(int width, int height, bool color = true, bool depth = false);
	void unframe(int width, int height);
};

bool operator<(texturehdl s0, texturehdl s1);
bool operator>(texturehdl s0, texturehdl s1);
bool operator<=(texturehdl s0, texturehdl s1);
bool operator>=(texturehdl s0, texturehdl s1);
bool operator==(texturehdl s0, texturehdl s1);
bool operator!=(texturehdl s0, texturehdl s1);

#endif
