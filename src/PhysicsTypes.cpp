/*
	PhysicsTypes.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 4/26/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PhysicsTypes.h"

Vector RotationalInertiaEquation(GLfloat Mass, Vertex *Points, GLint NumPoints)
{
	Vector I;
	Vertex Center;
	for (int c = 0; c < NumPoints; c++)
	{
		Center += Points[c];
	}
	Center /= NumPoints;
	
	for (int x = 0; x < NumPoints; x++)
	{
		I.x += Mass*sqr(Points[x].x - Center.x);
		I.y += Mass*sqr(Points[x].y - Center.y);
		I.z += Mass*sqr(Points[x].z - Center.z);
	}
	return I;
}

/*Vector TorqueEquation(Force F)
{
	return Cross(F.ApplicationPoint, F.ForceApplied);
}*/
