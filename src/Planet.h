/*
 *  Planet.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/13/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Planet_h
#define Planet_h

#include "Player.h"
#include "Model.h"
#include "CoreMathematics.h"
#include "CorePhysics.h"
#include "Material.h"
#include "OpenGLIncludes.h"

#define GridSize 100
#define WorldRes 50

struct Planet
{
	string Name;
	
	ModelPhysics Physics;
	Material	 PlanetTexture;
	Material	 AtmosphereTexture;
	Material	 GroundTexture;
	
	GLfloat grid_x, grid_y;
	
	unsigned char Roughness[WorldRes*WorldRes];
	unsigned char Heights[WorldRes*WorldRes];
	float *TempHeight;
		
	unsigned short sphere_indices[(WorldRes + 1)*(WorldRes + 1)*2];
	GLfloat		   sphere_verts[(WorldRes + 1)*(WorldRes + 1)*3];
	GLfloat		   sphere_texts[(WorldRes + 1)*(WorldRes + 1)*2];
	GLfloat		   sphere_norms[(WorldRes + 1)*(WorldRes + 1)*3];
	
	unsigned short grid_indices[(GridSize + 1)*(GridSize + 1)*2];
	GLfloat		   grid_verts[(GridSize + 1)*(GridSize + 1)*3];
	GLfloat		   grid_texts[(GridSize + 1)*(GridSize + 1)*3];
	GLfloat		   grid_norms[(GridSize + 1)*(GridSize + 1)*3];
	
	double Radius;
	double AtmRad;
	
	void CreateWorld();
	void Initialize(string name);
	
	void LoadPhysics(const char *filename);
	void LoadMaterials(const char *filename);
	void LoadPlanet(char *filename);
	void LoadGrids(GLfloat s, GLfloat t);
	
	void Render(Player *PlayerPointer);
	void Draw(Player *PlayerPointer);
	
	void Release();
};

#endif
