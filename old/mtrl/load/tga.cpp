/*
 * tga.cpp
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
 */

#include "tga.h"

bool load_tga(char *filename, imagehdl *tex)
{
	FILE	  	*file	= NULL;
	tga_header	header;
	int			bpp 	= 0;
	int		  	size	= 0;
	int			i, j;
	unsigned char rle;
	unsigned char color[4];

	file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Error: Unable to open %s.\n", filename);
		return false;
	}

	// Read all 18 bytes of the header
	fread(&header, sizeof(uint8_t), sizeof(tga_header), file);

	size = header.width * header.height;
	bpp  = header.bits_per_pixel/8;

	// Should be image type 2 (color) or type 10 (rle compressed color)
	if (header.data_type != 2 && header.data_type != 10)
	{
		fclose(file);
		printf("Error: Unsupported image type.\n");
		return false;
	}

	fseek(file, header.id_length, SEEK_CUR);

	// Check to make sure we are dealing with RGB or RGBA
	switch (bpp)
	{
	case 2:
		tex->size				= vec4i(header.width, header.height, 1, 1);
		tex->internal_format	= 4;
		tex->format				= GL_BGRA;
		tex->type				= GL_UNSIGNED_SHORT_5_5_5_1;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(unsigned short)];
		break;
	case 3:
		tex->size				= vec4i(header.width, header.height, 1, 1);
		tex->internal_format	= 3;
		tex->format				= GL_BGR;
		tex->type				= GL_UNSIGNED_BYTE;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(unsigned char)*3];
		break;
	case 4:
		tex->size				= vec4i(header.width, header.height, 1, 1);
		tex->internal_format	= 4;
		tex->format				= GL_BGRA;
		tex->type				= GL_UNSIGNED_BYTE;
		tex->data				= new unsigned char[tex->size[0]*tex->size[1]*tex->size[2]*tex->size[3]*sizeof(unsigned char)*4];
		break;
	default:
		fclose(file);
		printf("Error: Unsupported bpp.\n");
		return false;
	}

	switch (header.data_type)
	{
	case TGA_UNCOMPRESSED_RGB:
		fread(tex->data, bpp, size, file);
		break;
	case TGA_RLE_RGB:
		for (i = 0; i < size;)
		{
			// reads the RLE header
			fread(&rle, sizeof(unsigned char), 1, file);

			// if the rle header is below 128 it means that what follows is just raw data with rle+1 pixels
			if (rle < 128)
			{
				fread(&tex->data[i*bpp], bpp, rle+1, file);
				i += rle+1;
			}

			// If the rle header is equal or above 128 it means that we have a string of rle-127 pixels
			// That use the following pixels color
			else
			{
				// Read what color we should use
				fread(color, bpp, 1, file);

				// Insert the color stored in tmp into the following rle-127 pixels
				for (j = 0; j < rle-127; j++, i++)
				{
					// Because TGA file store their colors in BGRA format we need to swap the red and blue color components
					tex->data[i*bpp + 0] = color[0];
					tex->data[i*bpp + 1] = color[1];
					tex->data[i*bpp + 2] = color[2];
					if (bpp == 4)
						tex->data[i*bpp + 3] = color[3];
				}
			}
		}
		break;
	default:
		fclose(file);
		printf("Error: Unsupported data type.\n");
		return false;
	}

	fclose(file);

	return true;
}
