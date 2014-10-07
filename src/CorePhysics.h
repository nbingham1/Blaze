/*
 *  CorePhysics.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/7/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef CorePhysics_h
#define CorePhysics_h

#include "BasicPhysics.h"
#include "ModelPhysics.h"
#include <time.h>

struct CorePhysics
{
	CorePhysics()
	{
		PhysicsHandlers = NULL;
		start_time = time(0);
		num_frames = 0;
	}
	
	ModelPhysics *PhysicsHandlers;
	int num_frames;
	int start_time;
	
	void AddObjectPhysics(ModelPhysics *phs);
	void ApplyGravity();
	void UpdatePhysicsHandlers();
	void UpdatePhysicsHandlersAccAndVel();
	void ReleasePhysics();
};

#endif