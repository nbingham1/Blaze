/*
 *  Frustum.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Frustum_h
#define Frustum_h

#include <math.h>
#include "BasicMath.h"
#include "Vector.h"
#include "Plane.h"
#include "Box.h"
#include "Sphere.h"

struct Frustum
{
	Vector *Position;
	Vector *Orientation;
	GLdouble front, back, right, left, top, bottom;
	Vector Vertices[8];
	
	void Set(GLdouble f, GLdouble b, GLdouble r, GLdouble l, GLdouble t, GLdouble bo);
	//void AttachFrustumToEntity();
};

bool IntersectFrustumBox(Frustum f, Box b);
bool IntersectFrustumSphere(Frustum f, Sphere s);

#endif