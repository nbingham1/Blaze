/*
 *  CorePhysics.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "CorePhysics.h"

void CorePhysics::AddObjectPhysics(ModelPhysics *phs)
{
	if (PhysicsHandlers == NULL)
		PhysicsHandlers = phs;
	else
	{
		ModelPhysics *Current = PhysicsHandlers;
		while (Current->Next != NULL)
			Current = Current->Next;
		Current->Next = phs;
	}
}

void CorePhysics::ApplyGravity()
{
	GLdouble GravitationalPull;
	
	num_frames++;
	GLdouble secperframe = (GLdouble(time(0)-start_time)/GLdouble(num_frames));
		
	ModelPhysics *iter1, *iter2;
	iter1 = PhysicsHandlers;
	while (iter1 != NULL)
	{
		iter2 = iter1->Next;
		while (iter2 != NULL)
		{
			if (iter1 != iter2 && iter1->gravity && iter2->gravity)
			{
				GravitationalPull = CalculateGravitationalPull(iter1->Mass, iter2->Mass, iter1->Position, iter2->Position);
				iter1->Gravity = GravitationalPull*Normalize(iter2->Position-iter1->Position)*secperframe*secperframe;
				iter1->SumForces.Force += iter1->Gravity;
				iter2->Gravity = GravitationalPull*Normalize(iter1->Position-iter2->Position)*secperframe*secperframe;
				iter2->SumForces.Force += iter2->Gravity;
			}
			iter2 = iter2->Next;
		}
		iter1 = iter1->Next;
	}
	
	iter1 = PhysicsHandlers;
	while (iter1 != NULL)
	{
		iter1->gravity = true;
		iter1 = iter1->Next;
	}
}

void CorePhysics::UpdatePhysicsHandlers()
{
	ModelPhysics *iter1;
	iter1 = PhysicsHandlers;
	while (iter1 != NULL)
	{
		iter1->Update();
		iter1 = iter1->Next;
	}
}

void CorePhysics::UpdatePhysicsHandlersAccAndVel()
{
	ModelPhysics *iter1;
	iter1 = PhysicsHandlers;
	while (iter1 != NULL)
	{
		iter1->UpdateAccAndVel();
		iter1 = iter1->Next;
	}
}

void CorePhysics::ReleasePhysics()
{
}