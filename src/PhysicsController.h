/*
	PhysicsController.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/12/06.
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

#include "CoreMathematics.h"
#include "BasicDefinitions.h"

#ifndef PhysicsController_h
#define PhysicsController_h

struct PhysicsController
{
	Vector Position;
	Vector LinearVelocity;
	Vector LinearMomentum;
	
	Vector Orientation;
	Vector AngularVelocity;
	Vector AngularMomentum;
	
	GLfloat Mass;
	GLfloat Volume;
	GLfloat Elasticity;
	
	Vector CenterOfMass;
	Vector RotationalInertia;
	Vector Torque;
	
	bool Grounded;
		
	PhysicsController *Next;
		
	void Update();
	void Apply();
};

#endif
