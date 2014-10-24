/*
 *  Box.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Box.h"

void Box::SetBox(Vector Min, Vector Max)
{
	Vertices[0] = Min;
	Vertices[1] = Vector(Min.x, Min.y, Max.z);
	Vertices[2] = Vector(Max.x, Min.y, Max.z);
	Vertices[3] = Vector(Max.x, Min.y, Min.z);
	Vertices[4] = Vector(Min.x, Max.y, Min.z);
	Vertices[5] = Vector(Min.x, Max.y, Max.z);
	Vertices[6] = Max;
	Vertices[7] = Vector(Max.x, Max.y, Min.z);
}

bool IntersectBoxBox(Box b1, Box b2)
{
	Vector Box1[8], Box2[8];
	Matrix Rot1, Rot2;
	
	EulertoMatrix(*b1.Orientation, Rot1);
	EulertoMatrix(*b2.Orientation, Rot2);
	
	for (int x = 0; x < 8; x++)
	{
		Box1[x] = Rot1*b1.Vertices[x] + (*b1.Position);
		Box2[x] = Rot2*b2.Vertices[x] + (*b2.Position);
	}
	
	if (!Intersect(Box1, 8, Box2, 8, Vector(1.0, 0.0, 0.0)))
		return false;
	if (!Intersect(Box1, 8, Box2, 8, Vector(0.0, 1.0, 0.0)))
		return false;
	if (!Intersect(Box1, 8, Box2, 8, Vector(0.0, 0.0, 1.0)))
		return false;
		
	Vector boxedge1[] = {Box1[1] - Box1[0], Box1[2] - Box1[1], Box1[4] - Box1[0]};
	
	Vector boxedge2[] = {Box2[1] - Box2[0], Box2[2] - Box2[1], Box2[4] - Box2[0]};
	
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			if (!Intersect(Box1, 8, Box2, 8, Cross(boxedge1[x], boxedge2[y])))
				return false;
	
	return true;
}

