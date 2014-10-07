/*
 *  CoreGraphics.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */
 
#include "CoreMathematics.h"
#include "OpenGLIncludes.h"

#ifndef CoreGraphics_h
#define CoreGraphics_h

#include "Camera.h"
#include "Player.h"
#include "Planet.h"
#include "World.h"

struct CoreGraphics
{
	CorePhysics Physics;
	
	Player ThePlayer;
	World W;
	Planet P;
	
	void Init();
	void RenderFrame();
	void Release();
};

#endif
