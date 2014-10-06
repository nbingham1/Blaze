/*
	GameApp.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 8/6/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameApp.h"

void GameApp::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    refresh = 400;
    		
    glViewport(0, 0, 1400, 900);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1.0, 1.0, 2.0, 1000000000);
	gluLookAt(0.0, 10.0, 0.0, 0.0, 1.0, 1.0, 0.0, 2.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -23.0 / 2.5);

	//glEnable(GL_BLEND);
	//glEnable(GL_POINT_SMOOTH);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	
	GLfloat pos[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat spec[] = {10.0, 10.0, 10.0, 1.0};
	GLfloat ambi[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diff[] = {10.0, 10.0, 10.0, 1.0};
	
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, -1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, -1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
			
	m1.Load("res/Terrain/Terrain.obj", 200);
	World.Init(&m1, "res/m", "tga", 50000);
	
	PlayerMesh.Load("res/Player/Player.obj", 1);
	Player.Init(&PlayerMesh);
	Player.PlayerModel.Position.y = 1000;
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void GameApp::CleanUpApp()
{
	m1.Release();
	PlayerMesh.Release();
	World.Release();
}

void GameApp::Update()
{
	Input.HandleKeyStillDown();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Player.Prepare();
	Player.RenderCamera();
	GLfloat pos[] = {-1000, 100, 10000, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	World.Render();
	Player.RenderModel();
	glutSwapBuffers();
}

void GameApp::SetViewport(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1.0, 1.0, 2.0, 1000000000);
	gluLookAt(0.0, 10.0, 0.0, 0.0, 1.0, 1.0, 0.0, 2.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -23.0 / 2.5);
}
