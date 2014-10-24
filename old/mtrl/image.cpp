/*
 * image.cpp
 *
 *  Created on: Jun 26, 2012
 *      Author: Ned Bingham
 */

#include "image.h"

imagehdl::imagehdl()
{
	internal_format = 0;
	format			= GL_NONE;
	type			= GL_NONE;
	size			= vec4i();
	data			= NULL;
}

imagehdl::~imagehdl()
{
	if (data != NULL)
		delete [] data;

	internal_format = 0;
	format			= GL_NONE;
	type			= GL_NONE;
	size			= vec4i();
	data			= NULL;
}
