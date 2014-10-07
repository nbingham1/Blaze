/*
	CoreGraphics.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/8/06.
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


#include "CoreGraphics.h"

void CoreGraphics::Init()
{
	ThePlayer.Init();
	TheCamera.AttachCamera(Vector(0.0, ThePlayer.PlayerModel.Geometry->BoundingBox[6].y - ThePlayer.PlayerModel.Geometry->BoundingBox[0].y - .0003, -.0003));
	
	P.Init("ptest");
	//P.Physics->LinearVelocity.x = -.08;
	ThePlayer.PlayerModel.Physics->Position.z = -sqrt(2)*P.Radius*1.00094/2;
	ThePlayer.PlayerModel.Physics->Position.y = sqrt(2)*P.Radius*1.00094/2;
	//ThePlayer.PlayerModel.Physics->LinearVelocity.y = -.5;
			
	Physics.AddControllerToList(ThePlayer.PlayerModel.Physics);
	Physics.AddControllerToList(P.Physics);
}

void CoreGraphics::SetupPerspective()
{
	Perspective.SetFrustum(1, -1, 1.5, 1000000000, .625, -.625);
	Perspective.RenderFrustum();
	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	{
		GLfloat pos[] = {0.0, 1.0, 0.0, 0.0};
		GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat ambi[] = {0.0, 0.0, 0.0, 1.0};
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
	glEnable(GL_CULL_FACE);
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}
}

void CoreGraphics::DrawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clears the back buffer
	glLoadIdentity();
	
	s.SetScaleValue(ThePlayer.GridX, ThePlayer.GridY, ThePlayer.Planet, ThePlayer.System, ThePlayer.Galaxy);
	glScalef(s.scale_value, s.scale_value, s.scale_value);
	Physics.Update();
	ThePlayer.Prepare(&TheCamera);
	
	TheCamera.Render(&ThePlayer.PlayerModel);
	GLfloat pos[] = {0.0, 1.0, 0.0, 0.0};
	GLfloat spec[] = {s.scale_value, s.scale_value, s.scale_value, 1.0};
	GLfloat diff[] = {s.scale_value, s.scale_value, s.scale_value, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	P.Render(&ThePlayer);
	glFinish();
}

void CoreGraphics::Release()
{
	//Physics.Release();
	P.Release();
	ThePlayer.Release();
}
