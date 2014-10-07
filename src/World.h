/*
 *  World.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 4/30/07.
 *  Copyright 2007 Sol Union: Sol Gaming. All rights reserved.
 *
 */

#ifndef World_h
#define World_h

#include "Player.h"
#include "Model.h"
#include "CoreMathematics.h"
#include "CorePhysics.h"
#include "Material.h"
#include "OpenGLIncludes.h"

#define size_allotment 80000

struct World
{
	Player		*PlayerPointer;
	ModelPhysics Physics;
	Material	 Mat;
	
	GLfloat Radius;
	GLfloat Visual_Angle;
	GLfloat DispXAngle;
	GLfloat DispYAngle;
	
	unsigned int indices[size_allotment/4];
	GLfloat		 data[size_allotment];
	unsigned char *heights;
	int heightsize;
	unsigned char *roughness;
	int roughsize;
	
	void Setup(char *filename, GLfloat radius, Player *Pointer);
	void LoadMaterials(char *filename);
	void UpdateWorld();
	void Render();
	void Release();
};

#endif
