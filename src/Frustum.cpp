/*
 *  Frustum.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Frustum.h"

void Frustum::Set(GLdouble f, GLdouble b, GLdouble r, GLdouble l, GLdouble t, GLdouble bo)
{
	front = f;
	back = b;
	right = r;
	left = l;
	top = t;
	bottom = bo;

	Vertices[1] = Vector(left, bottom, front);
	Vertices[2] = Vector(right, bottom, front);
	Vertices[5] = Vector(left, top, front);
	Vertices[6] = Vector(right, top, front);

	Vertices[0] = back*Vertices[1]/front;
	Vertices[3] = back*Vertices[2]/front;
	Vertices[4] = back*Vertices[5]/front;
	Vertices[7] = back*Vertices[7]/front;
}

bool IntersectFrustumBox(Frustum f, Box b)
{
	Vector Box1[8], Box2[8];
	Matrix Rot1, Rot2;

	EulertoMatrix(*f.Orientation, Rot1);
	EulertoMatrix(*b.Orientation, Rot2);

	for (int x = 0; x < 8; x++)
	{
		Box1[x] = Rot1*f.Vertices[x] + (*f.Position);
		Box2[x] = Rot2*b.Vertices[x] + (*b.Position);
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

bool IntersectFrustumSphere(Frustum f, Sphere s)
{
	Vector frust[8];
	Matrix rot;

	EulertoMatrix(*f.Orientation, rot);

	for (int x = 0; x < 8; x++)
		frust[x] = rot*f.Vertices[x] + (*f.Position);

	Vector axis[4] = {f.Vertices[0] - f.Vertices[1], f.Vertices[3] - f.Vertices[2], f.Vertices[4] - f.Vertices[0], f.Vertices[7] - f.Vertices[4]};

	if (GetMin(frust, 8, axis[0]) > GetMax(s.Position, 1, axis[0]) + s.Radius)
		return false;
	if (GetMax(frust, 8, axis[0]) < GetMin(s.Position, 1, axis[0]) - s.Radius)
		return false;

	if (GetMin(frust, 8, axis[1]) > GetMax(s.Position, 1, axis[1]) + s.Radius)
		return false;
	if (GetMax(frust, 8, axis[1]) < GetMin(s.Position, 1, axis[1]) - s.Radius)
		return false;

	if (GetMin(frust, 8, axis[2]) > GetMax(s.Position, 1, axis[2]) + s.Radius)
		return false;
	if (GetMax(frust, 8, axis[2]) < GetMin(s.Position, 1, axis[2]) - s.Radius)
		return false;

	if (GetMin(frust, 8, axis[3]) > GetMax(s.Position, 1, axis[3]) + s.Radius)
		return false;
	if (GetMax(frust, 8, axis[3]) < GetMin(s.Position, 1, axis[3]) - s.Radius)
		return false;

	return true;
}
