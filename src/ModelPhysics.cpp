/*
 *  ModelPhysics.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "ModelPhysics.h"

void ModelPhysics::Update()
{
	LinearAccelaration = Vector(0.0, 0.0, 0.0);
	
	if (Mass != 0)
		LinearAccelaration = SumForces.Force/Mass;
		
	LinearVelocity += LinearAccelaration;
	Position += LinearVelocity;
	
	Torque = Cross(SumForces.AppPoint, SumForces.Force);
	
	AngularAccelaration = Torque/RotationalInertia;
	
	AngularVelocity += AngularAccelaration;
	Orientation += AngularVelocity;
	
	SumForces.Force = Vector(0.0, 0.0, 0.0);
	SumForces.AppPoint = Vector(0.0, 0.0, 0.0);
}

void ModelPhysics::Enable()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Orientation.x, 1.0, 0.0, 0.0);
	glRotatef(Orientation.y, 0.0, 1.0, 0.0);
	glRotatef(Orientation.z, 0.0, 0.0, 1.0);
}

void ModelPhysics::Disable()
{
	glPopMatrix();
}