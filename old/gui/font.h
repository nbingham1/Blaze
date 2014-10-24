/*
 * font.h
 *
 *  Created on: Aug 2, 2011
 *      Author: Ned Bingham
 */

#include "../graphics.h"

#ifndef font_h
#define font_h

struct fonthdl : materialhdl
{
	fonthdl();
	~fonthdl();

	void print(string str, rgba_hdr color, vec2f pos);
	void print(string str, rgba_hdr color, vec3f pos);

	void print(string str, rgb_hdr color, vec2f pos);
	void print(string str, rgb_hdr color, vec3f pos);
};

#endif
