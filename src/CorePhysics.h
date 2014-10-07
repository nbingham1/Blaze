/*
	CorePhysics.h
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

#ifndef CorePhysics_h
#define CorePhysics_h

#include "PhysicsController.h"

struct CorePhysics
{
	CorePhysics()
	{
		PhysicsList = NULL;
	}
	
	PhysicsController *PhysicsList;
	
	void AddControllerToList(PhysicsController *c);
	void Update();
	void Release();
};

Vector CalculateRotationalInertia(GLfloat Mass, Vector *Points, int NumPoints);
Vector CalculateTorque(Vector Force, Vector ApplicationPoint);
Vector CalculateCenterOfMass(Vector *Points, int NumPoints);
GLfloat CalculateDensity(GLfloat Volume, GLfloat Mass);
Vector CalculateMomentum(GLfloat Mass, Vector Velocity);
GLfloat CalculateGravitationalPull(GLfloat Mass1, GLfloat Mass2, Vector Position1, Vector Position2);

#endif
