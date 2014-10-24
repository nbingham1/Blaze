/*
 *  CorePhysics.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 11/7/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CorePhysics_h
#define CorePhysics_h

#include "PhysicsHandler.h"

struct CorePhysics
{
	PhsHandle *Handlers;
	PhsHandle *curr;
	int num_frame;
	int start_time;
	
	void Init();
	void AddPhsHandle(PhsHandle *phs);
	void ApplyGlobals();
	void Update();
	void Release();
};

#endif