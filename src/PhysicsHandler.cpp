/*
 *  PhysicsHandler.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "PhysicsHandler.h"

void PhsHandle::Initialize(GLdouble Mass, GLdouble Radius, GLdouble Elasticity)
{
	this->Mass = Mass;
	this->Elasticity = Elasticity;
	this->Inertia = 2.0*Mass*Radius*Radius/5.0;
	this->Radius = Radius;
	this->Volume = 4.0*pi*Radius*Radius*Radius/3.0;
	this->Density = Mass/this->Volume;
}

void PhsHandle::ApplyForce(Force f)
{
	SumForces += f.Magnitude;
	SumTorques += f.ToTorque();
}

void PhsHandle::ApplyForce(Vector mag, Vector pos)
{
	Force f;
	f.Magnitude = mag;
	f.AppliedPos = pos;
	
	SumForces += f.Magnitude;
	SumTorques += f.ToTorque();
}
	
void PhsHandle::UpdateLinear()
{
	LinearAccelaration = SumForces/Mass;
	LinearVelocity += LinearAccelaration;
	Position += LinearVelocity;
	
	LinearMomentum = Momentum(Mass, LinearVelocity);
	
	SumForces = Vector();
}

void PhsHandle::UpdateAngular()
{
	AngularAccelaration = SumTorques/Inertia;
	AngularVelocity += AngularAccelaration;
	Orientation += AngularVelocity;
	
	AngularMomentum = Momentum(Inertia, AngularVelocity);
	
	SumTorques = Vector();
}

void PhsHandle::Update()
{
	UpdateLinear();
	UpdateAngular();
}

void PhsHandle::EnableAngular()
{
	glPushMatrix();
	glRotatef(Orientation.x, 1.0, 0.0, 0.0);
	glRotatef(Orientation.y, 0.0, 1.0, 0.0);
	glRotatef(Orientation.z, 0.0, 0.0, 1.0);
}

void PhsHandle::DisableAngular()
{
	glPopMatrix();
}

void PhsHandle::EnableLinear()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
}

void PhsHandle::DisableLinear()
{
	glPopMatrix();
}

void PhsHandle::Enable()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Orientation.x, 1.0, 0.0, 0.0);
	glRotatef(Orientation.y, 0.0, 1.0, 0.0);
	glRotatef(Orientation.z, 0.0, 0.0, 1.0);
}

void PhsHandle::Disable()
{
	glPopMatrix();
}

void Gravity(PhsHandle *h1, PhsHandle *h2, GLfloat spf)
{
	Force f;
	f.AppliedPos = Vector(0.0, 0.0, 0.0);	
	double d = Distance(h1->Position, h2->Position);
		
	if (d == 0)
		f.Magnitude = Vector();
	else
		f.Magnitude = spf*spf*(h2->Position - h1->Position)*G*h1->Mass*h2->Mass/(d*d*d);
		
	h1->ApplyForce(f);
	f.Magnitude *= -1.0;
	h2->ApplyForce(f);
}

void Collide(PhsHandle *h1, PhsHandle *h2)
{
	Force f1, f2;
	f1.AppliedPos = Normalize(h2->Position - h1->Position)*h1->Radius;
	f2.AppliedPos = Normalize(h1->Position - h2->Position)*h2->Radius;
	f1.Magnitude = h2->Elasticity*h1->Elasticity*(h2->LinearMomentum - h1->LinearMomentum);
	f2.Magnitude = h2->Elasticity*h1->Elasticity*(h1->LinearMomentum - h2->LinearMomentum);
	
	h1->ApplyForce(f1);
	h2->ApplyForce(f2);
}