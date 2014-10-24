/*
 *  Mechanics.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "CoreMathematics.h"

#ifndef Mechanics_h
#define Mechanics_h

struct Force
{
	Vector Magnitude;
	Vector AppliedPos;
	
	Vector ToTorque();
};

Vector CenterOfMass(Vector *v, int n);
GLdouble Inertia(GLdouble m, Vector *v, int n);
GLdouble Density(GLdouble m, GLdouble v);
Vector Momentum(GLdouble m, Vector v);

const GLdouble G = 6.673*pow(10.0, -11.0);

#endif