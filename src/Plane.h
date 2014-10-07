/*
	Plane.h
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

#include "Vector.h"
#include "Matrix.h"
#include "BasicDefinitions.h"

#ifndef Plane_h
#define Plane_h

struct Plane
{
	Plane()
	{
	}
	Plane(Vector n, GLfloat d)
	{
		Normal = n;
		Distance = d;
	}
	Plane(GLfloat f[4])
	{
		Normal = Vector(f[0], f[1], f[2]);
		Distance = f[3];
	}
	Vector Normal;
	GLfloat Distance;
	
	Plane &operator =(Plane p);
};

Plane TriangleToPlane(Vector v1, Vector v2, Vector v3);
int ClassifyVertextoPlane(Plane p, Vector v);
int intersect_RayPlane(Vector R1, Vector R2, Plane p, Vector *I);

#endif
