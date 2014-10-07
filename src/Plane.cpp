/*
	Plane.cpp
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

#include "Plane.h"

Plane &Plane::operator =(Plane p)
{
	this->Normal = p.Normal;
	this->Distance = p.Distance;
	return *this;
}

Plane TriangleToPlane(Vector v1, Vector v2, Vector v3)
{
	Plane p;
	p.Normal = CalculateNormal(v1, v2, v3);
	p.Distance = -(Dot(p.Normal, v1));
	return p;
}

int ClassifyVertextoPlane(Plane p, Vector v)
{
	GLfloat D = Dot(v, p.Normal) + p.Distance;
	if (D > 0.00000001)
		return 0;
	else if (D < -0.00000001)
		return 1;
	else
		return 2;
}

int intersect_RayPlane(Vector R1, Vector R2, Plane p, Vector *I)
{
	Vector    u, v, n;
    Vector    dir, w0, w;
    GLfloat     r, a, b;

    dir = R2 - R1;
    w0 = R1 - Vector(0.0, p.Distance, 0.0);
    a = -Dot(p.Normal, w0);
    b = Dot(p.Normal, dir);

    if (fabs(b) < 0.00000001)
	{
        if (a == 0)
		{
            return 2;
		}
        else
		{
			return 0;
		}
    }

    r = a / b;
    if (r < 0.0)
	{
		return 0;
	}
	if (r > 1.0)
	{
		return 0;  
	}

    *I = R1 + dir * r;
	
	return 1;
}

