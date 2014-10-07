/*
	Triangle.h
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
#include "Plane.h"

#ifndef Triangle_h
#define Triangle_h

struct Triangle
{
	Triangle()
	{
	}
	Triangle(Vector v1, Vector v2, Vector v3)
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
	}
	
	Vector v[3];
	Vector n;
	
	int CollisionType;
	
	Triangle *Next;
	
	Triangle &operator =(Triangle t);
	void Split(Plane Splitter, Triangle *FrontList, Triangle *BackList);
};

int ClassifyTriangletoPlane(Plane p, Triangle *t);

#endif
