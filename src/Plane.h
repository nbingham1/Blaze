/*
 *  Plane.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Plane_h
#define Plane_h

#include <math.h>
#include "BasicMath.h"
#include "Vector.h"
#include "Box.h"
#include "Sphere.h"

struct Plane
{
	Vector Normal;
	GLdouble Distance;
};

int ClassifyVertexToPlane(Plane p, Vector v);
int ClassifyBoxToPlane(Plane p, Box b);
int ClassifySphereToPlane(Plane p, Sphere s);
int ClassifyPointCloudToPlane(Plane p, Vector *pc, int n);

bool RayPlaneIntersection(Vector r1, Vector r2, Plane p, Vector *i);

#endif
