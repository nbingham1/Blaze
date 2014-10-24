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
	glFrustum(-1.6/10.0, 1.6/10.0, -1.0/10.0, 1.0/10.0, .2, 999999999);
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
			
	Physics.Init();
		
	LoadObj(&viewmod, "res/Player/Player.obj");
	view.AttachCamera(&viewmod, Vector(0.0, 1.8, .25), true);
	view.SetMoveType(3);
	Physics.AddPhsHandle(&viewmod.Physics);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	p.Load("Emily", &view);
	p.Physics.Position = Vector(0.0, 0.0, 146000000000.0);
	Physics.AddPhsHandle(&p.Physics);
	s.Load("Sol", &view);
	view.Host->Physics.Position = Vector(0.0, 0.0, 146000000000.0 - p.Radius*2.0);
}

void CoreGraphics::RenderFrame()
{
	p.Prepare();
	s.Prepare();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clears the back buffer
	glLoadIdentity();
	
	Physics.Update();
	Physics.ApplyGlobals();
	
	view.Render();
	s.Render();
	p.Render();
	
	glFinish();
}

void CoreGraphics::Release()
{
	Physics.Release();
	ReleaseMdl(&viewmod);
	s.Release();
	p.Release();
}
