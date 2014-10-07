/*
 *  BasicPhysics.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/9/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "CoreMathematics.h"
#include "OpenGLIncludes.h"

#ifndef BasicPhysics_h
#define BasicPhysics_h

struct Force
{
	Vector Force;
	Vector AppPoint;
};

Vector CalculateRotationalInertia(GLfloat Mass, Vector *Points, int NumPoints);
Vector CalculateTorque(Vector Force, Vector ApplicationPoint);
Vector CalculateCenterOfMass(Vector *Points, int NumPoints);
GLfloat CalculateDensity(GLfloat Volume, GLfloat Mass);
Vector CalculateMomentum(GLfloat Mass, Vector Velocity);
double CalculateGravitationalPull(GLfloat Mass1, GLfloat Mass2, Vector Position1, Vector Position2);

#endif
