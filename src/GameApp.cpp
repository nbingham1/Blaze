/*
	GameApp.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 8/6/05.
	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GameApp.h"

void GameApp::Init()
{
	refresh = 60;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.333, 1.333, -1, 1, 2, 1000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);

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
		GLfloat pos[] = {0.0, 10000000, 0.0, 1.0};
		GLfloat spec[] = {10.0, 9.0, 9.0, 1.0};
		GLfloat ambi[] = {0.0, 0.0, 0.0, 1.0};
		GLfloat diff[] = {10.0, 9.0, 9.0, 1.0};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
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

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisotropy);

	glEnable(GL_CULL_FACE);
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}

	GLfloat fogcolor[] = {0.025, 0.2, 0.5, 1.0};
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.005);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);

	Player.Init(/*"Player/Player.wrl"*/ "res/Player/Player.obj");
	Player.PlayerModel.Physics.Position = Vector(0.0, 20000, 0.0);
	Environment.PlayerPos = &Player.PlayerModel.Physics.Position;
	
	Environment.InitWithSkyBox("res/m", "tga", 200000, "res/ptest.raw" /*"Terrain/Terrain.obj"*/, &Player.PlayerModel.Physics.Position);
	//Environment.InitWithSkyPlane("cloud.tga", 10, 60000, 100000, "ptest.raw", &Player.PlayerModel.Physics.Position);
	//Environment.InitWithSkyDome("cloud.tga", 50, 200000, 0.5, "ptest.raw", &Player.PlayerModel.Physics.Position);
	string files[] = {"res/ParticleSystem/Rain.part", "res/ParticleSystem/Snow.part", "res/ParticleSystem/Fire.part", "res/ParticleSystem/Dirt.part", "res/ParticleSystem/Grass.part", "res/ParticleSystem/Spark.part", "res/ParticleSystem/Smoke.part"};
	Particles.Init(7, files);
	
	Tree.Initialize();
	Tree.AddModelToData(&Environment.Map);
	Tree.AddModelToList(&Player.PlayerModel);
	Tree.AddModelToList(&Environment.t.TreeModel);
	Tree.Build();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void GameApp::CleanUpApp()
{
	Particles.Release();
	Tree.Release();
	Environment.Release();
	Player.Release();
}

void GameApp::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Input.HandleKeyStillDown();
	Tree.CheckForCollisions();
	Player.Prepare();
	if (Player.PlayerModel.Physics.Position.y < Environment.WaterLevel.vertices[1])
		glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
	else
		glFogi(GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE);

	Player.RenderCamera();
	GLfloat pos[] = {0.0, 100, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	Player.Render();

	Environment.Render(Vector(0.001, 0.0, 0.0));
	Particles.Render();

	glFlush();
	glutSwapBuffers();
}

void GameApp::SetViewport(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-GLfloat(w)/GLfloat(h), GLfloat(w)/GLfloat(h), -1.0, 1.0, 2.0, 1000000000);
	gluLookAt(0.0, 10.0, 0.0, 0.0, 1.0, 1.0, 0.0, 2.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -23.0 / 2.5);
}
