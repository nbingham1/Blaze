/*
	Environment.cpp
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
#include "Environment.h"
#include "CoreGraphics.h"

void Planet::CreateWorld()
{
	int n = MapSize/WorldRes;
	GLfloat angle;
	GLfloat PolyAngle = 2.0*pi/n;
	GLfloat vx, vy, vz;
	int index = 0;
	for (int x = 0; x <= n; x++)
	{
		angle = pi/n;
		for (int y = 0; y <= n; y++)
		{
			vx = cos(x*PolyAngle) * cos(y*angle - pi/2.0);
			vy = sin(y*angle - pi/2.0);
			vz = sin(x*PolyAngle) * cos(y*angle - pi/2.0);
			
			sphere_verts[(x * (n + 1) + y)*3 + 0] = vx*Radius;
			sphere_verts[(x * (n + 1) + y)*3 + 1] = vy*Radius;
			sphere_verts[(x * (n + 1) + y)*3 + 2] = vz*Radius;
			
			sphere_norms[(x * (n + 1) + y)*3 + 0] = vx;
			sphere_norms[(x * (n + 1) + y)*3 + 1] = vy;
			sphere_norms[(x * (n + 1) + y)*3 + 2] = vz;
			
			sphere_texts[(x * (n + 1) + y)*2 + 0] = GLfloat(x)/GLfloat(n);
			sphere_texts[(x * (n + 1) + y)*2 + 1] = GLfloat(n-y)/GLfloat(n);
		}
	}
	
	for (int x = 1; x <= n; x++)
	{
		for (int y = 0; y <= n; y++)
		{
			sphere_indices[index++] = x*(n+1)+y;
			sphere_indices[index++] = (x-1)*(n+1)+y;
		}
	}
}

void Planet::LoadGrids(GLfloat s, GLfloat t)
{
	CurrentX = s;
	CurrentY = t;
	
	GLfloat angle;
	GLfloat PolyAngle = 2.0*pi/NumGrids;
	GLfloat vx, vy, vz;
	int index = 0;
	for (int x = 0; x <= GridSize; x++)
	{
		angle = pi/NumGrids;
		for (int y = 0; y <= GridSize; y++)
		{
			vx = cos(((GLfloat(x)/GLfloat(GridSize)) + s)*PolyAngle) * cos(((GLfloat(y)/GLfloat(GridSize)) + t)*angle - pi/2.0);
			vy = sin(((GLfloat(y)/GLfloat(GridSize)) + t)*angle - pi/2.0);
			vz = sin(((GLfloat(x)/GLfloat(GridSize)) + s)*PolyAngle) * cos(((GLfloat(y)/GLfloat(GridSize)) + t)*angle - pi/2.0);
			
			grid_verts[(x * (GridSize + 1) + y)*3 + 0] = vx*(Radius + Radius*.001*Heights[(int(s*GridSize) + x) * MapSize + (int(t*GridSize) + y)]/255);
			grid_verts[(x * (GridSize + 1) + y)*3 + 1] = vy*(Radius + Radius*.001*Heights[(int(s*GridSize) + x) * MapSize + (int(t*GridSize) + y)]/255);
			grid_verts[(x * (GridSize + 1) + y)*3 + 2] = vz*(Radius + Radius*.001*Heights[(int(s*GridSize) + x) * MapSize + (int(t*GridSize) + y)]/255);
			
			grid_norms[(x * (GridSize + 1) + y)*3 + 0] = vx;
			grid_norms[(x * (GridSize + 1) + y)*3 + 1] = vy;
			grid_norms[(x * (GridSize + 1) + y)*3 + 2] = vz;
			
			grid_texts[(x * (GridSize + 1) + y)*2 + 0] = ((s*GridSize) + x)*10;
			grid_texts[(x * (GridSize + 1) + y)*2 + 1] = ((t*GridSize) + y)*10;
		}
	}
	
	for (int x = 1; x <= GridSize; x++)
	{
		for (int y = 0; y <= GridSize; y++)
		{
			grid_indices[index++] = x*(GridSize+1)+y;
			grid_indices[index++] = (x-1)*(GridSize+1)+y;
		}
	}
}

void Planet::RenderWorld(GLuint tex, GLfloat mult)
{
	int n = MapSize/WorldRes;
	glEnable(GL_TEXTURE_2D);
	glScalef(mult, mult, mult);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, sphere_texts);
	glVertexPointer(3, GL_FLOAT, 0, sphere_verts);
	glNormalPointer(GL_FLOAT, 0, sphere_norms);
	glBindTexture(GL_TEXTURE_2D, tex);
	for (int x = 0; x < n; x++)
		glDrawElements(GL_QUAD_STRIP, (n+1)*2, GL_UNSIGNED_SHORT, &sphere_indices[x*(n+1)*2]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glScalef(1/mult, 1/mult, 1/mult);
	glDisable(GL_TEXTURE_2D);
}

void Planet::RenderGrids()
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, grid_texts);
	glVertexPointer(3, GL_FLOAT, 0, grid_verts);
	glNormalPointer(GL_FLOAT, 0, grid_norms);
	glBindTexture(GL_TEXTURE_2D, GroundTexture);
	for (int x = 0; x < GridSize; x++)
		glDrawElements(GL_QUAD_STRIP, (GridSize+1)*2, GL_UNSIGNED_SHORT, &grid_indices[x*(GridSize+1)*2]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void Planet::Init(string Name)
{
	PlanetName = Name;
	
	Physics = (PhysicsController*)malloc(sizeof(PhysicsController));
	
	Radius = (MapSize*GridScale)/3.545;
	Physics->Mass = 5515*((4/3)*pi*Radius*Radius*Radius*1000000000);
	AtmosphereSize = Radius*.1881;
	
	Physics->Position = Vector(0.0, 0.0, 0.0);
	Physics->LinearVelocity = Vector(0.0, 0.0, 0.0);

	NumGrids = int(MapSize/GridSize);
	
	if (MapSize/GridSize > NumGrids)
		NumGrids++;
	if (MapSize/GridSize > NumGrids)
		NumGrids++;
	
	Texture = Load2DTexture("res/Earth.tga", false);
	GroundTexture = Load2DTexture("res/GrassSample.tga", true);
	SkyTexture = Load2DTexture("res/clouds.tga", false);
	
	FILE *file;
	file = fopen(("res/" + PlanetName + ".raw").c_str(), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(("res/" + PlanetName + ".raw").c_str());
		return;
	}
	
	fread(Heights, 1, MapSize*MapSize, file);
	
	fclose(file);
	
	CreateWorld();
	LoadGrids(0, 0);
	
	Next = NULL;
	GridChange = false;
}

void Planet::Render(Player *player)
{
	glPushMatrix();
	Physics->Apply();
	
	GLfloat d = Distance(player->PlayerModel.Physics->Position, Physics->Position);
	if (d <= Radius + AtmosphereSize + 1)
		glClearColor(0.0, 0.3*(1.0 - ((d - Radius)/AtmosphereSize)), 1.0 - ((d - Radius)/AtmosphereSize), 0.0);
	if (d <= Radius*1.00188)
	{
		Vector p = Normalize(player->PlayerModel.Physics->Position);
		
		GLfloat y_angle = asin(p.y) + pi/2.0;
		GLfloat x_angle = acos(p.x/cos(y_angle - pi/2.0));
		
		if (p.z > 0)
			x_angle = -x_angle + 2.0*pi;
		
		x_angle /= 2*pi/NumGrids;
		y_angle /= pi/NumGrids;
		x_angle = NumGrids - x_angle;
		
		GLfloat u = x_angle-.5 - GLfloat(CurrentX);
		GLfloat v = y_angle-.5 - GLfloat(CurrentY);
		if (u < 0)
			u *= -1;
		if (v < 0)
			v *= -1;
		
		if (u >= 0.25 || v >= 0.25)
		{
			LoadGrids(x_angle - .5, y_angle-.5);
			GridChange = true;
		}
		else
			GridChange = false;
		
		player->GridX = (int)x_angle;
		player->GridY = (int)y_angle;
		player->Planet = 1;
		player->System = 1;
		player->Galaxy = 1;
		
		RenderGrids();
	}
	else
		GridChange = false;
	
	glDisable(GL_CULL_FACE);
	if (d > Radius*1.00188)
		RenderWorld(Texture, 1);
	RenderWorld(SkyTexture, 1.00094);
	//glEnable(GL_CULL_FACE);
	
	if (d > Radius*1.00188 && d <= Radius + 2000)
	{
		player->GridX = -1;
		player->GridY = -1;
		player->Planet = 1;
		player->System = 1;
		player->Galaxy = 1;
	}
	else if (d > Radius + 2000)
	{
		player->GridX = -1;
		player->GridY = -1;
		player->Planet = -1;
		player->System = 1;
		player->Galaxy = 1;
	}
	glPopMatrix();
}

void Planet::Release()
{
	if (Physics)
	{
		free(Physics);
		Physics = NULL;
	}
	glDeleteTextures(1, &Texture);
	glDeleteTextures(1, &GroundTexture);
	glDeleteTextures(1, &SkyTexture);
}

void Star::Init(GLfloat Radius)
{
	this->Radius = Radius;
	Physics = (PhysicsController*)malloc(sizeof(PhysicsController));
	Physics->Mass = 141000*((4/3)*pi*Radius*Radius*Radius*1000000000);
	Next = NULL;
}

void Star::Render()
{
	glPushMatrix();
	Physics->Apply();
	glDisable(GL_LIGHTING);
	glColor4f(1, 1, .5, 1.0);
	//glDisable(GL_CULL_FACE);
	glutSolidSphere(Radius, 20, 20);
	//glEnable(GL_CULL_FACE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Star::Release()
{
	if (Physics)
	{
		free(Physics);
		Physics = NULL;
	}
}

void System::Init()
{
	Physics = (PhysicsController*)malloc(sizeof(PhysicsController));
}

void System::NewPlanet(string Name)
{
	Planet *new_planet = (Planet*)malloc(sizeof(Planet));
	new_planet->Init(Name);
	
	if (PlanetList == NULL)
		PlanetList = new_planet;
	else
	{
		Planet *Current = PlanetList;
		while (Current->Next != NULL)
			Current = Current->Next;
		Current->Next = new_planet;
	}
}

void System::NewStar(GLfloat Radius)
{
	Star *new_star = (Star*)malloc(sizeof(Star));
	new_star->Init(Radius);
	
	if (StarList == NULL)
		StarList = new_star;
	else
	{
		Star *Current = StarList;
		while (Current->Next != NULL)
			Current = Current->Next;
		Current->Next = new_star;
	}
}

void System::Render(Vector *Position)
{
	glPushMatrix();
	Physics->Apply();
	
	Planet *CurrentPlanet = PlanetList;
	while (CurrentPlanet != NULL)
	{
		//CurrentPlanet->Render(Position);
		CurrentPlanet = CurrentPlanet->Next;
	}
	
	Star *CurrentStar = StarList;
	while (CurrentStar != NULL)
	{
		CurrentStar->Render();
		CurrentStar = CurrentStar->Next;
	}
	
	glPopMatrix();
}

void System::Release()
{
	Planet *CurrentPlanet = PlanetList, *PreviousPlanet;
	while (CurrentPlanet != NULL)
	{
		PreviousPlanet = CurrentPlanet;
		CurrentPlanet = CurrentPlanet->Next;
		PreviousPlanet->Release();
		free(PreviousPlanet);
	}
	
	Star *CurrentStar = StarList, *PreviousStar;
	while (CurrentStar != NULL)
	{
		PreviousStar = CurrentStar;
		CurrentStar = CurrentStar->Next;
		PreviousStar->Release();
		free(PreviousStar);
	}
	
	if (Physics)
	{
		free(Physics);
		Physics = NULL;
	}
}
