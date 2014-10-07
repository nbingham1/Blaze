/*
	CorePhysics.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 11/5/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CorePhysics.h"

void CorePhysics::AddControllerToList(PhysicsController *c)
{
	c->Next = NULL;
	if (PhysicsList == NULL)
		PhysicsList = c;
	else
	{
		PhysicsController *temp = PhysicsList;
		while (temp->Next != NULL)
			temp = temp->Next;
		temp->Next = c;
	}
}

void CorePhysics::Update()
{
	PhysicsController *Current1, *Current2;
	GLfloat gravity;
	Current1 = PhysicsList;
	while (Current1 != NULL)
	{
		Current2 = PhysicsList;
		while (Current2 != NULL)
		{
			if (Current1 != Current2)
			{
				gravity = CalculateGravitationalPull(Current1->Mass, Current2->Mass, Current1->Position, Current2->Position);
				Current1->LinearVelocity += gravity*Normalize(Current2->Position-Current1->Position)/Current1->Mass;
				Current2->LinearVelocity += gravity*Normalize(Current1->Position-Current1->Position)/Current2->Mass;
			}
			Current2 = Current2->Next;
		}
		Current1 = Current1->Next;
	}
	
	Current1 = PhysicsList;
	while (Current1 != NULL)
	{
		Current1->Update();
		Current1 = Current1->Next;
	}
}

void CorePhysics::Release()
{
	PhysicsController *Current = PhysicsList, *Previous;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		free(Previous);
		Previous = NULL;
	}
}

Vector CalculateRotationalInertia(GLfloat Mass, Vector *Points, int NumPoints)
{
	Vector I;
	Vector Center = CalculateCenterOfMass(Points, NumPoints);
	
	for (int x = 0; x < NumPoints; x++)
	{
		I.x += Mass*Square(Points[x].x - Center.x);
		I.y += Mass*Square(Points[x].y - Center.y);
		I.z += Mass*Square(Points[x].z - Center.z);
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

GLfloat CalculateDensity(GLfloat Volume, GLfloat Mass)
{
	return Mass/Volume;
}

Vector CalculateMomentum(GLfloat Mass, Vector Velocity)
{
	return Velocity*Mass;
}

GLfloat CalculateGravitationalPull(GLfloat Mass1, GLfloat Mass2, Vector Position1, Vector Position2)
{
	GLfloat r = Distance(Position1, Position2)*1000;
	return ((6.673*pow(10, -11) * Mass1 * Mass2)/(r*r))/3600000;
}

