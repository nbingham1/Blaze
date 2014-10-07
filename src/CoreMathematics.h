/*
	CoreMathematics.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/18/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CoreMathematics_h
#define CoreMathematics_h

#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Plane.h"
#include "Triangle.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef pi
#define pi 3.1415926535898
#endif

#ifndef Square
#define Square(n) n*n
#endif

#ifndef DegToRad
#define DegToRad(d) (pi/180)*d
#endif

#ifndef RadToDeg
#define RadToDeg(r) (180/pi)*r
#endif

#ifndef Clamp
#define Clamp(x, min, max)	((x = (x < min ? min : x < max ? x : max)))
#endif

#endif
