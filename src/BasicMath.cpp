/*
 *  BasicMath.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "BasicMath.h"

double GetMin(Vector *Points, int Num, Vector Axis)
{
	double min = 9E99;
	
	for (int ctr = 0; ctr < Num; ctr++)
    {
		double dotprod = Dot(Points[ctr], Axis);
		if (dotprod < min)
			min = dotprod;
    }
	return min;
}

double GetMax(Vector *Points, int Num, Vector Axis)
{
	double max = -9E99;
	
	for (int ctr = 0; ctr < Num; ctr++)
    {
		double dotprod = Dot(Points[ctr], Axis);
		if (dotprod > max)
			max = dotprod;
    }
	return max;
}

GLdouble absolute(GLdouble f)
{
	return (f < 0.0) ? (f*-1) : f;
}

GLdouble modulate(GLdouble f, GLdouble m)
{
	GLdouble h = f - GLdouble(int(f/m))*m;
	return h < 0.0 ? h+m : h;
}

bool Intersect(Vector *p1, int n1, Vector *p2, int n2, Vector Axis)
{
	if (GetMin(p1, n1, Axis) > GetMax(p2, n2, Axis))
		return false;
	if (GetMax(p1, n1, Axis) < GetMin(p2, n2, Axis))
		return false;
		
	return true;     
}

bool IntersectBoxTri(Vector BoxPoints[16], Vector v1, Vector v2, Vector v3)
{
	Vector v[3] = {v1, v2, v3};
	Vector Normal = CalculateNormal(v1, v2, v3);
	// test the x, y, and z axes
	if (!Intersect(BoxPoints, 16, v, 3, Vector(1, 0, 0)))
		return false;
	if (!Intersect(BoxPoints, 16, v, 3, Vector(0, 1, 0)))
		return false;
	if (!Intersect(BoxPoints, 16, v, 3, Vector(0, 0, 1)))
		return false;
	if (!Intersect(BoxPoints, 16, v, 3, Normal))
		return false;
	
	// test the triangle normal
	Vector triedge[] = {v2 - v1, v3 - v2, v1 - v3};
	
	Vector boxedge[] = {BoxPoints[4]-BoxPoints[0], BoxPoints[1]-BoxPoints[0],
						BoxPoints[3]-BoxPoints[0], BoxPoints[12]-BoxPoints[8],
						BoxPoints[9]-BoxPoints[8], BoxPoints[11]-BoxPoints[8],
						BoxPoints[8]-BoxPoints[0], BoxPoints[9]-BoxPoints[1],
						BoxPoints[10]-BoxPoints[2], BoxPoints[11]-BoxPoints[3],
						BoxPoints[12]-BoxPoints[4], BoxPoints[13]-BoxPoints[5],
						BoxPoints[14]-BoxPoints[6], BoxPoints[15]-BoxPoints[7]};
	
	for (int x = 0; x < 14; x++)
		for (int y = 0; y < 3; y++)
			if (!Intersect(BoxPoints, 16, v, 3, Cross(boxedge[x], triedge[y])))
				return false;
	
	
	return true;
	
}
