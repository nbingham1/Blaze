/*
 * image.h
 *
 *  Created on: Jun 26, 2012
 *      Author: Ned Bingham
 */

#include "../common.h"
#include "../math.h"

#ifndef image_h
#define image_h

struct imagehdl
{
	imagehdl();
	~imagehdl();

	char	file[256];
	int		internal_format;
	GLenum	format;
	GLenum	type;
	vec4i	size;
	uint8_t	*data;
};

#endif
