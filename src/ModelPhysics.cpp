/*
 *  ModelPhysics.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "ModelPhysics.h"

void ModelPhysics::UpdateAccAndVel()
{
	LinearAccelaration = Vector(0.0, 0.0, 0.0);
	
	if (Mass != 0.0)
		LinearAccelaration = SumForces.Force/Mass;
		
	LinearVelocity += LinearAccelaration;
	
	LinearMomentum = LinearVelocity*Mass;
	
	Torque = Cross(SumForces.AppPoint, SumForces.Force);
	
	AngularAccelaration = Torque/RotationalInertia;
	
	AngularVelocity += AngularAccelaration;
	
	SumForces.Force = Vector(0.0, 0.0, 0.0);
	SumForces.AppPoint = Vector(0.0, 0.0, 0.0);
}

void ModelPhysics::Update()
{
	LinearAccelaration = Vector(0.0, 0.0, 0.0);
	
	if (Mass != 0.0)
		LinearAccelaration = SumForces.Force/Mass;
		
	LinearVelocity += LinearAccelaration;
	LinearMomentum = LinearVelocity*Mass;
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
	glTranslated(Position.x, Position.y, Position.z);
	glRotated(Orientation.x, 1.0, 0.0, 0.0);
	glRotated(Orientation.y, 0.0, 1.0, 0.0);
	glRotated(Orientation.z, 0.0, 0.0, 1.0);
}

void ModelPhysics::Disable()
{
	glPopMatrix();
}