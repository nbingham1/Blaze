/*
 *  Box.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Box_h
#define Box_h

#include <math.h>
#include "Vector.h"
#include "BasicMath.h"

struct Box
{
	Vector *Position;
	Vector *Orientation;
	Vector Vertices[8];

	void SetBox(Vector Min, Vector Max);
	void SetBox(GLdouble x_size, GLdouble y_size, GLdouble z_size);
	//void AttachBoxToEntity();
};

bool IntersectBoxBox(Box b1, Box b2);

#endif
