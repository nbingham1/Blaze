/*
 *  Sphere.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Sphere_h
#define Sphere_h

#include <math.h>
#include "BasicMath.h"
#include "Vector.h"
#include "Box.h"

struct Sphere
{
	Vector *Position;
	GLdouble Radius;
	//void AttachSphereToEntity();
};

bool IntersectSphereSphere(Sphere s1, Sphere s2);
bool IntersectSphereBox(Sphere s, Box b);

#endif
