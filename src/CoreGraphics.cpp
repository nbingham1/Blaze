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
	glFrustum(-1.0, 1.0, -5.0/8.0, 5.0/8.0, 12.0/8.0, 500000000);
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
	glEnable(GL_LIGHTING);
	{
		GLfloat pos[] =  {1.0, 0.0, 0.0, 0.0};
		GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat ambi[] = {0.3, 0.3, 0.3, 1.0};
		GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};
		
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glEnable(GL_LIGHT0);
		{
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
			glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, -1);
		}
	}
	/*glEnable(GL_CULL_FACE);
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}*/
	
	glEnable(GL_NORMALIZE);
	
	ThePlayer.Init("res/Player1/Player.obj", 3);
	P.Initialize("Planet");
	
	Physics.AddObjectPhysics(&ThePlayer.PlayerModel.Physics);
	Physics.AddObjectPhysics(&P.Physics);
	
	//W.Setup("res/heights.raw", 6380000, &ThePlayer);
	//W.Physics.Position = 6500000*Normalize(Vector(-1.0, 0.00001, 0.00001));
	//W.UpdateWorld();
	
	P.Physics.Position = 6500000*Normalize(Vector(-1.0, 0.00001, 0.00001));
}

void CoreGraphics::RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clears the back buffer
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	Physics.UpdatePhysicsHandlers();
	Physics.ApplyGravity();
	
	ThePlayer.Render();
		
	GLfloat pos[] = {.97, 0.24, 0.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	
	//W.UpdateWorld();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//W.Render();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	P.Render(&ThePlayer);
	glFinish();
}

void CoreGraphics::Release()
{
	Physics.ReleasePhysics();
	ThePlayer.Release();
	//W.Release();
	P.Release();
}
