/*
	GameTexture.h
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/18/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Main.h"

#ifndef GameTexture_h
#define GameTexture_h

struct rgba_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct rgb_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct texture_t
{
	short	Scale;
	long	Width;
	long	Height;
	GLuint	Image;
	long	Depth;
};

texture_t LoadTexture(const char* filename, GLboolean MipMap, GLenum format, GLint internalFormat, short imageScale);

#endif
