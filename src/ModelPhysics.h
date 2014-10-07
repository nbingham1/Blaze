/*
 *  ModelPhysics.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef ModelPhysics_h
#define ModelPhysics_h

#include "CoreMathematics.h"
#include "BasicPhysics.h"

struct ModelPhysics
{
	ModelPhysics()
	{
		Mass = 0.0;
		Volume = 0.0;
		Density = 0.0;
		Elasticity = 0.0;
		
		Next = NULL;
		gravity = true;
	}

	double Mass;
	double Volume;
	double Density;
	double Elasticity;

	Vector Position;
	Vector LinearVelocity;
	Vector LinearAccelaration;
	
	Vector Orientation;
	Vector AngularVelocity;
	Vector AngularAccelaration;
	
	Vector RotationalInertia;
	Vector Torque;
	
	Force  SumForces;
	bool   gravity;
	
	ModelPhysics *Next;
	
	void Update();
	
	void Enable();
	void Disable();
};

#endif
