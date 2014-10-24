/*
 * color.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 19, 2012.
 * Modified by Ned Bingham on June 19, 2012.
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

#include "../math.h"
#include "../common.h"

#ifndef color_h
#define color_h

typedef vec<uint8_t, 3> rgb8;
typedef vec<uint8_t, 4> rgba8;

typedef vec<uint16_t, 3> rgb16;
typedef vec<uint16_t, 4> rgba16;

typedef vec<uint32_t, 3> rgb32;
typedef vec<uint32_t, 4> rgba32;

typedef vec<float, 3> rgbf;
typedef vec<float, 4> rgbaf;

#endif
