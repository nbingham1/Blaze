/*
	Mesh.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/12/06.
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

#include "Mesh.h"

void MakeBox(Vector Min, Vector Max, Box TheBox)
{
	TheBox[0] = Min;
	TheBox[1] = Vector(Min.x, Min.y, Max.z);
	TheBox[2] = Vector(Max.x, Min.y, Max.z);
	TheBox[3] = Vector(Max.x, Min.y, Min.z);
	TheBox[4] = Vector(Min.x, Max.y, Min.z);
	TheBox[5] = Vector(Min.x, Max.y, Max.z);
	TheBox[6] = Max;
	TheBox[7] = Vector(Max.x, Max.y, Min.z);
}

GLfloat getmin(Vector *points, int NumPoints, Vector axis)
{
	GLfloat min = 99999999999.0;
	
	for (int ctr = 0; ctr < NumPoints; ctr++)
    {
		GLfloat dotprod = Dot(points[ctr], axis);
		if (dotprod < min)
			min = dotprod;
    }
	return min;
}

GLfloat getmax(Vector *points, int NumPoints, Vector axis)
{
	GLfloat max = -99999999999.0;
	
	for (int ctr = 0; ctr < NumPoints; ctr++)
    {
		GLfloat dotprod = Dot(points[ctr], axis);
		if (dotprod > max)
			max = dotprod;
    }
	return max;
}

bool isect(Vector *P1, int PNum1, Vector *P2, int PNum2, Vector axis)
{
	if (getmin(P1, PNum1, axis) > getmax(P2, PNum2, axis))
		return false;
	if (getmax(P1, PNum1, axis) < getmin(P2, PNum2, axis))
		return false;
		
	return true;     
}

bool isectboxbox(Vector Box1[16], Vector Box2[16])
{
	// test the x, y, and z axes
	if (!isect(Box1, 16, Box2, 16, Vector(1, 0, 0)))
		return false;
	if (!isect(Box1, 16, Box2, 16, Vector(0, 1, 0)))
		return false;
	if (!isect(Box1, 16, Box2, 16, Vector(0, 0, 1)))
		return false;
	
	// test the triangle normal
	
	Vector boxedge1[] = {Box1[4]-Box1[0], Box1[1]-Box1[0],
						Box1[3]-Box1[0], Box1[12]-Box1[8],
						Box1[9]-Box1[8], Box1[11]-Box1[8],
						Box1[8]-Box1[0], Box1[9]-Box1[1],
						Box1[10]-Box1[2], Box1[11]-Box1[3],
						Box1[12]-Box1[4], Box1[13]-Box1[5],
						Box1[14]-Box1[6], Box1[15]-Box1[7]};
	
	Vector boxedge2[] = {Box2[4]-Box2[0], Box2[1]-Box2[0],
						Box2[3]-Box2[0], Box2[12]-Box2[8],
						Box2[9]-Box2[8], Box2[11]-Box2[8],
						Box2[8]-Box2[0], Box2[9]-Box2[1],
						Box2[10]-Box2[2], Box2[11]-Box2[3],
						Box2[12]-Box2[4], Box2[13]-Box2[5],
						Box2[14]-Box2[6], Box2[15]-Box2[7]};
	
	for (int x = 0; x < 14; x++)
		for (int y = 0; y < 14; y++)
			if (!isect(Box1, 16, Box2, 16, Cross(boxedge1[x], boxedge2[y])))
				return false;
	
	
	return true;
}

bool isectboxtri(Vector BoxPoints[16], Triangle TrianglePoints)
{
	Vector Normal = TrianglePoints.n;
	// test the x, y, and z axes
	if (!isect(BoxPoints, 16, TrianglePoints.v, 3, Vector(1, 0, 0)))
		return false;
	if (!isect(BoxPoints, 16, TrianglePoints.v, 3, Vector(0, 1, 0)))
		return false;
	if (!isect(BoxPoints, 16, TrianglePoints.v, 3, Vector(0, 0, 1)))
		return false;
	if (!isect(BoxPoints, 16, TrianglePoints.v, 3, Normal))
		return false;
	
	// test the triangle normal
	Vector triedge[] = {TrianglePoints.v[1] - TrianglePoints.v[0], TrianglePoints.v[2] - TrianglePoints.v[1], TrianglePoints.v[0] - TrianglePoints.v[2]};
	
	Vector boxedge[] = {BoxPoints[4]-BoxPoints[0], BoxPoints[1]-BoxPoints[0],
						BoxPoints[3]-BoxPoints[0], BoxPoints[12]-BoxPoints[8],
						BoxPoints[9]-BoxPoints[8], BoxPoints[11]-BoxPoints[8],
						BoxPoints[8]-BoxPoints[0], BoxPoints[9]-BoxPoints[1],
						BoxPoints[10]-BoxPoints[2], BoxPoints[11]-BoxPoints[3],
						BoxPoints[12]-BoxPoints[4], BoxPoints[13]-BoxPoints[5],
						BoxPoints[14]-BoxPoints[6], BoxPoints[15]-BoxPoints[7]};
	
	for (int x = 0; x < 14; x++)
		for (int y = 0; y < 3; y++)
			if (!isect(BoxPoints, 16, TrianglePoints.v, 3, Cross(boxedge[x], triedge[y])))
				return false;
	
	
	return true;
	
}
