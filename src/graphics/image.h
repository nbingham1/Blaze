/*
 * image.h
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include <std/string.h>

using namespace core;

#ifndef image_h
#define image_h

struct imagehdl
{
	imagehdl();
	imagehdl(string filename);
	~imagehdl();

	string data;
	int bpp;
	int width;
	int height;
	int depth;

	bool load(string filename);

	bool load_tga(string filename);
};


#endif
