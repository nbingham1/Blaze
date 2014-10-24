/*
 * img.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 17, 2012.
 * Modified by Ned Bingham on June 17, 2012.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This code was derived from http://eeg.sourceforge.net/ANALYZE75.pdf
 */

#include "img.h"
#include "../../math.h"
#include "../../input.h"
#include "../texture.h"

bool load_img_hdr(char *filename, img_header *header)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: Could not open %s", filename);
		return false;
	}

	fread(header, 1, sizeof(img_header), file);

	fclose(file);

	return true;
}

bool load_img(char *filename, imagehdl *tex)
{
	img_header header;

	char path[256];
	char name[256];
	char headername[256];

	fpath(path, filename);
	fname(name, filename);
	sprintf(headername, "%s/%s.hdr", path, name);

	if (!load_img_hdr(headername, &header))
		return false;

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: Could not open %s", filename);
		return false;
	}

	uint32_t total_elements = 1;
	for (int i = 1; i < header.dim.dim[0]; i++)
		total_elements *= header.dim.dim[i];

	switch (header.dim.datatype)
	{
	case IMG_UNSIGNED_CHAR:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 1;
		tex->format				= GL_LUMINANCE;
		tex->type				= GL_UNSIGNED_BYTE;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(unsigned char)];
		break;
	case IMG_SIGNED_SHORT:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 1;
		tex->format				= GL_LUMINANCE;
		tex->type				= GL_SHORT;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(short)];
		break;
	case IMG_SIGNED_INT:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 1;
		tex->format				= GL_LUMINANCE;
		tex->type				= GL_INT;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(int)];
		break;
	case IMG_FLOAT:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 1;
		tex->format				= GL_LUMINANCE;
		tex->type				= GL_FLOAT;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(float)];
		break;
	case IMG_COMPLEX:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 2;
		tex->format				= GL_LUMINANCE_ALPHA;
		tex->type				= GL_FLOAT;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(complexf)];
		break;
	case IMG_DOUBLE:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 1;
		tex->format				= GL_LUMINANCE;
		tex->type				= GL_DOUBLE;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(double)];
		break;
	case IMG_RGB:
		tex->size				= vec4i(&header.dim.dim[1]);
		tex->internal_format	= 3;
		tex->format				= GL_RGB;
		tex->type				= GL_UNSIGNED_BYTE;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(unsigned char)*3];
		break;
	default:
		printf("Error: Data type not specified\n");
		return false;
		break;
	}

	fseek(file, header.dim.vox_offset, SEEK_SET);
	fread(tex, 1, total_elements, file);

	fclose(file);

	return true;
}
