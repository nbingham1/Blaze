/*
 * tga.h
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

#include "../../common.h"
#include "../image.h"

#ifndef tga_h
#define tga_h

/* Image type (data_type)
 *
 * CMI   - "Color Mapped Image"
 * RGB   - "Red, Green, and Blue"
 * BW    - "Black and White"
 * RLE   - "Run Length Encoded"
 * HDRLE - "Huffman, Delta, and Run Length Encoded"
 * QUAD  - "4 pass quadtree-type process"
 */
#define TGA_NONE						0
#define TGA_UNCOMPRESSED_CMI			1
#define TGA_UNCOMPRESSED_RGB			2
#define TGA_UNCOMPRESSED_BW				3
#define TGA_RLE_CMI						9
#define TGA_RLE_RGB			 			10
#define TGA_COMPRESSED_BW				11
#define TGA_COMPRESSED_CMI_HDRLE		32
#define TGA_COMPRESSED_CMI_HDRLE_QUAD	33

struct tga_header
{
   uint8_t  id_length;
   uint8_t  colour_map_type;
   uint8_t  data_type;
   uint16_t colour_map_origin;
   uint16_t colour_map_length;
   uint8_t  colour_map_depth;
   uint16_t x_origin;
   uint16_t y_origin;
   uint16_t width;
   uint16_t height;
   uint8_t  bits_per_pixel;
   uint8_t  image_descriptor;
};

bool load_tga(char *filename, imagehdl *tex);

#endif
