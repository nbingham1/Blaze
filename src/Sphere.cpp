/*
 *  Sphere.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Sphere.h"

bool IntersectSphereSphere(Sphere s1, Sphere s2)
{
	double distance = Distance(*s1.Position, *s2.Position);
	
	if (distance <= s1.Radius + s2.Radius)
		return true;
	else
		return false;
}

bool IntersectSphereBox(Sphere s, Box b)
{
	Vector box[8];
	Matrix rot;
	
	EulertoMatrix(*b.Orientation, rot);
	
	for (int x = 0; x < 8; x++)
		box[x] = rot*b.Vertices[x] + (*b.Position);
		
	Vector axis[4] = {b.Vertices[0] - b.Vertices[1], b.Vertices[3] - b.Vertices[2], b.Vertices[4] - b.Vertices[0], b.Vertices[7] - b.Vertices[4]};
		
	if (GetMin(box, 8, axis[0]) > Dot(*s.Position, axis[0]) + s.Radius)
		return false;
	if (GetMax(box, 8, axis[0]) < Dot(*s.Position, axis[0]) - s.Radius)
		return false;
	
	if (GetMin(box, 8, axis[1]) > Dot(*s.Position, axis[1]) + s.Radius)
		return false;
	if (GetMax(box, 8, axis[1]) < Dot(*s.Position, axis[1]) - s.Radius)
		return false;
	
	if (GetMin(box, 8, axis[2]) > Dot(*s.Position, axis[2]) + s.Radius)
		return false;
	if (GetMax(box, 8, axis[2]) < Dot(*s.Position, axis[2]) - s.Radius)
		return false;
		
	if (GetMin(box, 8, axis[3]) > Dot(*s.Position, axis[3]) + s.Radius)
		return false;
	if (GetMax(box, 8, axis[3]) < Dot(*s.Position, axis[3]) - s.Radius)
		return false;
		
	return true;
}