/*
 *  BasicPhysics.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/9/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "BasicPhysics.h"

Vector CalculateRotationalInertia(GLdouble Mass, Vector *Points, int NumPoints)
{
	Vector I;
	Vector Center = CalculateCenterOfMass(Points, NumPoints);
	
	for (int x = 0; x < NumPoints; x++)
	{
		I.x += Mass*(Points[x].x - Center.x)*(Points[x].x - Center.x);
		I.y += Mass*(Points[x].y - Center.y)*(Points[x].y - Center.y);
		I.z += Mass*(Points[x].z - Center.z)*(Points[x].z - Center.z);
	}
	return I;
}

Vector CalculateTorque(Vector Force, Vector ApplicationPoint)
{
	return Cross(ApplicationPoint, Force);
}

Vector CalculateCenterOfMass(Vector *Points, int NumPoints)
{
	Vector Center;
	for (int c = 0; c < NumPoints; c++)
	{
		Center += Points[c];
	}
	return Center/NumPoints;
}

GLdouble CalculateDensity(GLdouble Volume, GLdouble Mass)
{
	return Mass/Volume;
}

Vector CalculateMomentum(GLdouble Mass, Vector Velocity)
{
	return Velocity*Mass;
}

GLdouble CalculateGravitationalPull(GLdouble Mass1, GLdouble Mass2, Vector Position1, Vector Position2)
{
	GLdouble r = Distance(Position1, Position2);
	return ((6.673*pow(10, -11) * Mass1 * Mass2)/(r*r));
}