/*
 *  Plane.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Plane.h"

int ClassifyVertexToPlane(Plane p, Vector v)
{
	GLdouble distance = Dot(v, p.Normal) + p.Distance;

	if (distance > 0.000001)
		return 0;
	else if (distance < -0.000001)
		return 1;
	else
		return 2;
}

int ClassifyBoxToPlane(Plane p, Box b)
{
	int c[3] = {0, 0, 0};

	Vector Box[8];
	Matrix Rot;

	EulertoMatrix(*b.Orientation, Rot);

	for (int x = 0; x < 8; x++)
	{
		Box[x] = Rot*b.Vertices[x] + (*b.Position);
		c[ClassifyVertexToPlane(p, Box[x])]++;
	}
	if (c[1] == 0)
		return 0;
	else if (c[0] == 0)
		return 1;
	else
		return 3;
}

int ClassifySphereToPlane(Plane p, Sphere s)
{
	GLdouble distance = Dot(*s.Position, p.Normal) + p.Distance;
	GLdouble abs_distance = distance;
	if (abs_distance < 0)
		abs_distance *= -1;

	if (distance > 0.000001 && abs_distance > s.Radius)
		return 0;
	else if (distance < -0.000001 && abs_distance > s.Radius)
		return 0;
	else
		return 3;
}

int ClassifyPointCloudToPlane(Plane p, Vector *pc, int n)
{
	int c[3] = {0, 0, 0};

	for (int x = 0; x < n; x++)
		c[ClassifyVertexToPlane(p, pc[x])]++;

	if (c[1] == 0)
		return 0;
	else if (c[0] == 0)
		return 1;
	else if (c[2] == n)
		return 2;
	else
		return 3;
}

bool RayPlaneIntersection(Vector r1, Vector r2, Plane p, Vector *i)
{
	Vector    u, v, n;
    Vector    dir, w0, w;
    GLdouble     r, a, b;

    dir = r2 - r1;
    w0 = r1 - Vector(0.0, p.Distance, 0.0);
    a = -Dot(p.Normal, w0);
    b = Dot(p.Normal, dir);

    if (fabs(b) < 0.00000001)
        return false;

    r = a / b;
    if (r < 0.0 || r > 1.0)
		return false;

    *i = r1 + dir * r;

	return true;
}
