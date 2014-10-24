/*
 *  CorePhysics.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "CorePhysics.h"

void CorePhysics::Init()
{
	Handlers = NULL;
	start_time = time(0);
	num_frame = 60;
}

void CorePhysics::AddPhsHandle(PhsHandle *phs)
{
	if (Handlers == NULL)
	{
		Handlers = phs;
		curr = Handlers;
		curr->Next = NULL;
	}
	else
	{
		curr->Next = phs;
		curr = curr->Next;
		curr->Next = NULL;
	}
}

void CorePhysics::ApplyGlobals()
{
	num_frame++;
	GLfloat spf = GLfloat(time(0) - start_time)/GLfloat(num_frame);
	
	//cout << 1.0/spf << endl;
	
	PhsHandle *c1 = Handlers, *c2 = Handlers;
	while (c1 != NULL)
	{
		c2 = c1->Next;
		while (c2 != NULL)
		{
			Gravity(c1, c2, spf);
			
			//if (Distance(c1->Position, c2->Position) < c1->Radius + c2->Radius && (Dot(c2->LinearVelocity, c1->Position-c2->Position) >= 0 || Dot(c1->LinearVelocity, c2->Position-c1->Position) >= 0))
			//	Collide(c1, c2);
			c2 = c2->Next;
		}
		c1 = c1->Next;
	}
}

void CorePhysics::Update()
{
	PhsHandle *c = Handlers;
	while (c != NULL)
	{
		c->Update();
		c = c->Next;
	}
}

void CorePhysics::Release()
{
	PhsHandle *prev;
	curr =  Handlers;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->Next;
		prev->Next = NULL;
	}
}