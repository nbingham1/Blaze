/*
	Environment.h
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/17/06.
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

#include "BasicDefinitions.h"
#include "CoreMathematics.h"
#include "Model.h"
#include "Lighting.h"
#include "CorePhysics.h"
#include "Player.h"

#ifndef Environment_h
#define Environment_h

#define MapSize 128
#define GridSize 128
#define WorldRes 2
#define GridScale 8.8365
//8.8365

struct Grid
{
	Model *HeightMapModel;
		
	void ReLoadGrid(string filename, string mtlname, string phsname, int x_start, int z_start, int TotalSize);
	void GenerateRandomGrid();
	void SaveGrid(string filename);
	void Render();
	void Release();
};

struct Planet
{
	bool GridChange;
	string PlanetName;
	GLuint Texture;
	GLuint GroundTexture;
	GLuint SkyTexture;
	
	char Heights[MapSize*MapSize];
	
	unsigned short sphere_indices[((MapSize/WorldRes) + 1)*((MapSize/WorldRes) + 1)*6];
	GLfloat sphere_verts[((MapSize/WorldRes) + 1)*((MapSize/WorldRes) + 1)*3];
	GLfloat sphere_texts[((MapSize/WorldRes) + 1)*((MapSize/WorldRes) + 1)*2];
	GLfloat sphere_norms[((MapSize/WorldRes) + 1)*((MapSize/WorldRes) + 1)*3];
	
	int NumGrids;
	GLfloat CurrentX;
	GLfloat CurrentY;
		
	unsigned short grid_indices[(GridSize+1)*(GridSize+1)*6];
	GLfloat grid_verts[(GridSize+1)*(GridSize+1)*3];
	GLfloat grid_texts[(GridSize+1)*(GridSize+1)*2];
	GLfloat grid_norms[(GridSize+1)*(GridSize+1)*3];
	
	PhysicsController *Physics;
	GLfloat Radius;
	GLfloat AtmosphereSize;
	
	Planet *Next;
	
	void CreateWorld();
	void LoadGrids(GLfloat s, GLfloat t);
	void RenderWorld(GLuint tex, GLfloat mult);
	void RenderGrids();
	void Init(string Name);
	void Render(Player *player);
	void Release();
};

struct Star
{
	string StarName;
	
	GLuint Texture;
	
	GLfloat Radius;
	PhysicsController *Physics;
	
	Star *Next;
	
	void Init(GLfloat Radius);
	void Render();
	void Release();
};

struct System
{
	System()
	{
		PlanetList = NULL;
		StarList = NULL;
	}
	
	Planet *PlanetList;
	Star   *StarList;
	PhysicsController *Physics;
	
	void Init();
	void NewPlanet(string Name);
	void NewStar(GLfloat Radius);
	void Render(Vector *Position);
	void Release();
};

#endif
