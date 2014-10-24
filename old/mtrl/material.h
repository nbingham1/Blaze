/*
 * material.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 8, 2012.
 * Modified by Ned Bingham on June 8, 2012.
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

#include "texture.h"
#include "shader.h"
#include "../common.h"

#ifndef material_h
#define material_h

struct materialhdl : generichdl
{
	texturehdl texture;

	GLuint vertex;
	GLuint fragment;
	GLuint program;
};

#endif
