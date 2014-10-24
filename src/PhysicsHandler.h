/*
 *  PhysicsHandler.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Mechanics.h"

#ifndef PhysicsHandler_h
#define PhysicsHandler_h

struct PhsHandle
{
	GLdouble Mass;	// in kg
	GLdouble Inertia; // in kg*m^2

	GLdouble Radius; // in m
	GLdouble Volume;	// in m^3
	GLdouble Density; // in kg/m^3

	GLdouble Elasticity;

	Vector SumForces;

	Vector Position;
	Vector LinearVelocity;
	Vector LinearAccelaration;
	Vector LinearMomentum;

	Vector SumTorques;

	Vector Orientation;
	Vector AngularVelocity;
	Vector AngularAccelaration;
	Vector AngularMomentum;

	PhsHandle *Next;

	void Initialize(GLdouble Mass, GLdouble Radius, GLdouble Elasticity);

	void ApplyForce(Force f);
	void ApplyForce(Vector mag, Vector pos);

	void UpdateLinear();
	void UpdateAngular();
	void Update();

	void EnableAngular();
	void DisableAngular();

	void EnableLinear();
	void DisableLinear();

	void Enable();
	void Disable();
};

void Gravity(PhsHandle *h1, PhsHandle *h2, GLfloat spf);
void Collide(PhsHandle *h1, PhsHandle *h2);

#endif
