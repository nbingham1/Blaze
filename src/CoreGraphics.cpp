/*
 *  CoreGraphics.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "CoreGraphics.h"

void CoreGraphics::Init()
{	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.6, 1.6, -1.0, 1.0, 2.0, 999999999999.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.5);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	glEnable(GL_CULL_FACE);
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}
	
	
	ThePlayer.Init("res/Player1/Player.obj", 3);
	
	Physics.AddObjectPhysics(&ThePlayer.PlayerModel.Physics);
	
	n.Load("Ned", &ThePlayer);
	Physics.AddObjectPhysics(&n.Physics);
	ThePlayer.PlayerModel.Physics.Position.z += n.data.radius*1200;
}

void CoreGraphics::RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clears the back buffer
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	ThePlayer.Render();
	
	Physics.ApplyGravity();
	Physics.UpdatePhysicsHandlersAccAndVel();
	n.Render();
	Physics.UpdatePhysicsHandlers();
	
	glFinish();
}

void CoreGraphics::Release()
{
	Physics.ReleasePhysics();
	ThePlayer.Release();
	n.Release();
}
