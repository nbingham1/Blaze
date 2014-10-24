/*
 *  Mechanics.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Mechanics.h"

Vector Force::ToTorque()
{
	return Cross(AppliedPos, Magnitude);
}

Vector CenterOfMass(Vector *v, int n)
{
	Vector c;
	for (int x = 0; x < n; x++)
	{
		c += v[x];
	}
	return c/GLdouble(n);
}

GLdouble Inertia(GLdouble m, Vector *v, int n)
{
	GLdouble I;
	Vector Center = CenterOfMass(v, n);
	GLdouble d;
	for (int x = 0; x < n; x++)
	{
		d = Distance(v[x], Center);
		I += m*d*d;
	}
	
	return I;
}

GLdouble Density(GLdouble m, GLdouble v)
{
	return m/v;
}

Vector Momentum(GLdouble m, Vector v)
{
	return m*v;
}
