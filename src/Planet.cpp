/*
 *  Planet.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 1/13/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Planet.h"
#include "Input.h"

void Planet::CreateWorld()
{
	GLfloat angle;
	GLfloat PolyAngle = 2.0*pi/WorldRes;
	GLfloat vx, vy, vz;
	int index = 0;
	for (int x = 0; x <= WorldRes; x++)
	{
		angle = pi/WorldRes;
		for (int y = 0; y <= WorldRes; y++)
		{
			vx = cos(x*PolyAngle) * cos(y*angle - pi/2.0);
			vy = sin(y*angle - pi/2.0);
			vz = sin(x*PolyAngle) * cos(y*angle - pi/2.0);
			
			sphere_verts[(x * (WorldRes + 1) + y)*3 + 0] = vx*Radius;
			sphere_verts[(x * (WorldRes + 1) + y)*3 + 1] = vy*Radius;
			sphere_verts[(x * (WorldRes + 1) + y)*3 + 2] = vz*Radius;
			
			sphere_norms[(x * (WorldRes + 1) + y)*3 + 0] = vx;
			sphere_norms[(x * (WorldRes + 1) + y)*3 + 1] = vy;
			sphere_norms[(x * (WorldRes + 1) + y)*3 + 2] = vz;
			
			sphere_texts[(x * (WorldRes + 1) + y)*2 + 0] = GLfloat(WorldRes-x)/GLfloat(WorldRes);
			sphere_texts[(x * (WorldRes + 1) + y)*2 + 1] = GLfloat(WorldRes-y)/GLfloat(WorldRes);
		}
	}
	
	for (int x = 1; x <= WorldRes; x++)
	{
		for (int y = 0; y <= WorldRes; y++)
		{
			sphere_indices[index++] = x*(WorldRes+1)+y;
			sphere_indices[index++] = (x-1)*(WorldRes+1)+y;
		}
	}
}

void Planet::Initialize(string name)
{
	Name = name;
	
	PlanetTexture.Initialize();
	AtmosphereTexture.Initialize();
	
	LoadPlanet("");
	
	CreateWorld();
	grid_x = 500.0;
	grid_y = 500.0;
}

void Planet::LoadPhysics(const char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	Physics.Mass = 1.0;
	Physics.Elasticity = 1.0;
	Physics.RotationalInertia = Vector(1.0, 1.0, 1.0);
	
	file = fopen(filename, "r");
	
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "Rs", 2) == 0)
		{
			sscanf(oneline, "Rs %f", &v1);
			Radius = v1;
			Physics.Mass = 6000*(4/3)*pi*pow(Radius, 3);
			AtmRad = Radius*.1881;
		}
		
		if (strncmp(oneline, "El", 2) == 0)
		{
			sscanf(oneline, "El %f", &v1);
			Physics.Elasticity = v1;
		}
		
		if (strncmp(oneline, "Po", 2) == 0)
		{
			sscanf(oneline, "Po %f %f %f", &v1, &v2, &v3);
			Physics.Position.x = v1;
			Physics.Position.y = v2;
			Physics.Position.z = v3;
		}
		
		if (strncmp(oneline, "Or", 2) == 0)
		{
			sscanf(oneline, "Or %f %f %f", &v1, &v2, &v3);
			Physics.Orientation.x = v1;
			Physics.Orientation.y = v2;
			Physics.Orientation.z = v3;
		}
		
		if (strncmp(oneline, "Ms", 2) == 0)
		{
			sscanf(oneline, "Ms %f", &v1);
			Physics.Mass = v1;
		}
	}

	fclose(file);
		
	Physics.Volume = (4/3)*pi*pow(Radius, 3);
	Physics.Density = Physics.Mass/Physics.Volume;
}

void Planet::LoadMaterials(const char *filename)
{
	FILE *file = fopen(filename, "r");
	char oneline[256];
	GLfloat v1, v2, v3;
	int mat = 0;
	char Name[32];
	
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "newmtl", 6) == 0)
		{
			sscanf(oneline, "newmtl %s", Name);
			if (strncmp(Name, "PlanetTexture", 13) == 0)
			{
				mat = 0;
				PlanetTexture.Initialize();
			}
			else if (strncmp(Name, "AtmosphereTexture", 17) == 0)
			{
				mat = 1;
				AtmosphereTexture.Initialize();
			}
			else if (strncmp(Name, "GroundTexture", 13) == 0)
			{
				mat = 2;
				GroundTexture.Initialize();
			}
		}
		
		if (strncmp(oneline, "Am", 2) == 0)
		{
			sscanf(oneline, "Am %f %f %f", &v1, &v2, &v3);
			if (mat == 0)
			{
				PlanetTexture.Ambient.x = v1;
				PlanetTexture.Ambient.y = v2;
				PlanetTexture.Ambient.z = v3;
			}
			else if (mat == 1)
			{
				AtmosphereTexture.Ambient.x = v1;
				AtmosphereTexture.Ambient.y = v2;
				AtmosphereTexture.Ambient.z = v3;
			}
			else if (mat == 2)
			{
				GroundTexture.Ambient.x = v1;
				GroundTexture.Ambient.y = v2;
				GroundTexture.Ambient.z = v3;
			}
		}
		
		if (strncmp(oneline, "Di", 2) == 0)
		{
			sscanf(oneline, "Di %f %f %f", &v1, &v2, &v3);
			if (mat == 0)
			{
				PlanetTexture.Diffuse.x = v1;
				PlanetTexture.Diffuse.y = v2;
				PlanetTexture.Diffuse.z = v3;
			}
			else if (mat == 1)
			{
				AtmosphereTexture.Diffuse.x = v1;
				AtmosphereTexture.Diffuse.y = v2;
				AtmosphereTexture.Diffuse.z = v3;
			}
			else if (mat == 2)
			{
				GroundTexture.Diffuse.x = v1;
				GroundTexture.Diffuse.y = v2;
				GroundTexture.Diffuse.z = v3;
			}
		}
		
		if (strncmp(oneline, "Sp", 2) == 0)
		{
			sscanf(oneline, "Sp %f %f %f", &v1, &v2, &v3);
			if (mat == 0)
			{
				PlanetTexture.Specular.x = v1;
				PlanetTexture.Specular.y = v2;
				PlanetTexture.Specular.z = v3;
			}
			else if (mat == 1)
			{
				AtmosphereTexture.Specular.x = v1;
				AtmosphereTexture.Specular.y = v2;
				AtmosphereTexture.Specular.z = v3;
			}
			else if (mat == 2)
			{
				GroundTexture.Specular.x = v1;
				GroundTexture.Specular.y = v2;
				GroundTexture.Specular.z = v3;
			}
		}
		
		if (strncmp(oneline, "Em", 2) == 0)
		{
			sscanf(oneline, "Em %f %f %f", &v1, &v2, &v3);
			if (mat == 0)
			{
				PlanetTexture.Emission.x = v1;
				PlanetTexture.Emission.y = v2;
				PlanetTexture.Emission.z = v3;
			}
			else if (mat == 1)
			{
				AtmosphereTexture.Emission.x = v1;
				AtmosphereTexture.Emission.y = v2;
				AtmosphereTexture.Emission.z = v3;
			}
			else if (mat == 2)
			{
				GroundTexture.Emission.x = v1;
				GroundTexture.Emission.y = v2;
				GroundTexture.Emission.z = v3;
			}
		}
		
		if (strncmp(oneline, "Op", 2) == 0)
		{
			sscanf(oneline, "Op %f", &v1);
			if (mat == 0)
				PlanetTexture.Opacity = v1;
			else if (mat == 1)
				AtmosphereTexture.Opacity = v1;
			else if (mat == 2)
				GroundTexture.Opacity = v1;
		}
		
		if (strncmp(oneline, "Re", 2) == 0)
		{
			sscanf(oneline, "Re %f", &v1);
			if (mat == 0)
				PlanetTexture.Shininess = v1;
			else if (mat == 1)
				AtmosphereTexture.Shininess = v1;
			else if (mat == 2)
				GroundTexture.Shininess = v1;
		}
		
		if (strncmp(oneline, "TextMap2D", 9) == 0)
		{
			char LOD[5];
			
			if (mat == 0)
			{
				sscanf(oneline, "TextMap2D %s %s", PlanetTexture.TextName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					PlanetTexture.TextMap = Load2DTexture(string(PlanetTexture.TextName), true);
				else
					PlanetTexture.TextMap = Load2DTexture(string(PlanetTexture.TextName), false);
			}
			else if (mat == 1)
			{
				sscanf(oneline, "TextMap2D %s %s", AtmosphereTexture.TextName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					AtmosphereTexture.TextMap = Load2DTexture(string(AtmosphereTexture.TextName), true);
				else
					AtmosphereTexture.TextMap = Load2DTexture(string(AtmosphereTexture.TextName), false);
			}
			else if (mat == 2)
			{
				sscanf(oneline, "TextMap2D %s %s", GroundTexture.TextName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					GroundTexture.TextMap = Load2DTexture(string(GroundTexture.TextName), true);
				else
					GroundTexture.TextMap = Load2DTexture(string(GroundTexture.TextName), false);
			}
		}
		
		else if (strncmp(oneline, "TextMap3D", 9) == 0 && mat == 2)
		{
			char LOD[5];
						
			short TextureDepth;
			char filetype[6];
			
			sscanf(oneline, "TextMap3D %d %s %s %s", &TextureDepth, GroundTexture.TextName, filetype, LOD);
						
			string a[TextureDepth];
			for (int y = 0; y < TextureDepth; y++)
				a[y] = string(GroundTexture.TextName) + "0" + string(int_to_char(y)) + string(filetype);
			
			GroundTexture.TextDimension = 3;
			
			if (strncmp(LOD, "LOD", 3) == 0)
				GroundTexture.TextMap = Load3DTexture(a, TextureDepth, true);
			else
				GroundTexture.TextMap = Load3DTexture(a, TextureDepth, false);
		}
			
		if (strncmp(oneline, "DetailMap2D", 11) == 0)
		{
			char LOD[5];
			
			if (mat == 0)
			{
				sscanf(oneline, "DetailMap2D %s %s", PlanetTexture.DetailName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					PlanetTexture.DetailMap = Load2DTexture(string(PlanetTexture.DetailName), true);
				else
					PlanetTexture.DetailMap = Load2DTexture(string(PlanetTexture.DetailName), false);
			}
			else if (mat == 1)
			{
				sscanf(oneline, "DetailMap2D %s %s", AtmosphereTexture.DetailName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					AtmosphereTexture.DetailMap = Load2DTexture(string(AtmosphereTexture.DetailName), true);
				else
					AtmosphereTexture.DetailMap = Load2DTexture(string(AtmosphereTexture.DetailName), false);
			}
			else if (mat == 2)
			{
				sscanf(oneline, "DetailMap2D %s %s", GroundTexture.DetailName, LOD);
				if (strncmp(LOD, "LOD", 3) == 0)
					GroundTexture.DetailMap = Load2DTexture(string(GroundTexture.DetailName), true);
				else
					GroundTexture.DetailMap = Load2DTexture(string(GroundTexture.DetailName), false);
			}
		}
	}
	fclose(file);
}

void Planet::LoadPlanet(char *filename)
{
	string filen = string(filename);
	filen += "res/" + Name + "/" + Name + ".";
	string mtl = filen + "mtl";
	string phs = filen + "phs";
	
	LoadMaterials(mtl.c_str());
	LoadPhysics(phs.c_str());
	
	FILE *file;
	file = fopen("res/boundary.raw", "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf("res/boundary.raw");
		return;
	}
	
	fread(Roughness, 1, WorldRes*WorldRes, file);
	
	fclose(file);
	
	file = fopen("res/heights.raw", "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf("res/heights.raw");
		return;
	}
	
	fread(Heights, 1, WorldRes*WorldRes, file);
	
	fclose(file);
}

int Detail(int x, int p)
{
	return int(2000.0*pow(x-p, 3.0)/(pow(GridSize-p, 3.0)+pow(p, 3.0))+1000.0);
}

GLfloat DetailInv(GLfloat x, int p)
{
	GLfloat f = ((x-1000.0)/2000.0)*(pow(GLfloat(GridSize-p), 3.0)+pow(GLfloat(p), 3.0));
	if (f > 0.0)
		return pow(f, .33333333333) + GLfloat(p);
	else if (f < 0.0)
		return -pow(-f, .333333333333) + GLfloat(p);
	else
		return GLfloat(p);
}

GLfloat dDetail(GLfloat x, int p)
{
	return GLfloat(2000.0*pow(x-p, 3.0)/(pow(GridSize-p, 3.0)+pow(p, 3.0))+1000.0);
}

void Planet::LoadGrids(GLfloat s, GLfloat t)
{
	grid_x = s;
	grid_y = t;
	
	if (grid_x >= WorldRes)
		grid_x -= WorldRes;
	if (grid_y >= WorldRes)
		grid_y -= WorldRes;

	GLfloat angle = 2.0*pi/WorldRes;
	GLfloat vx, vy, vz;
	int index = 0;
	GLfloat x_percent, y_percent;
	
	GLfloat Temperature = .6*cos(pi*t/WorldRes - pi/2);
	Temperature = .6 - (Temperature - .2);
	
	delete TempHeight;
	
	TempHeight = GenerateFaultHeights(s*t, Detail(GridSize+1, GridSize/2), 100, -4000, 1000, 0.9);
	//TempHeight = GenerateRandomHeights(s*t, Detail(GridSize+1, GridSize/2), 3000, GLfloat(Roughness[int(s)*WorldRes + int(t)])/256.0, 0.0); //16819*GLfloat(Heights[int(s)*WorldRes + int(t)])/256.0 - 10924.0

	GLfloat Max = -99999999.0;
	GLfloat Min = 99999999.0;
	
	for (int x = 0; x < Detail(GridSize + 1, GridSize/2)*Detail(GridSize + 1, GridSize/2); x++)
	{
		if (TempHeight[x] > Max)
			Max = TempHeight[x];
		if (TempHeight[x] < Min)
			Min = TempHeight[x];
	}
	
	for (int x = 0; x < (GridSize+1)*(GridSize+1); x++)
	{
		grid_norms[(x)*3 + 0] = 0.0;
		grid_norms[(x)*3 + 1] = 0.0;
		grid_norms[(x)*3 + 2] = 0.0;
	}
	
	Vector v1, v2, v3, n;
	for (int x = 0; x <= GridSize; x++)
	{
		for (int y = 0; y <= GridSize; y++)
		{
			x_percent = (GLfloat(Detail(x, GridSize/2))/GLfloat(Detail(GridSize, GridSize/2)));
			y_percent = (GLfloat(Detail(y, GridSize/2))/GLfloat(Detail(GridSize, GridSize/2)));
			vx = cos((x_percent + s)*angle) * cos((y_percent + t)*angle - pi/2.0);
			vy = sin((y_percent + t)*angle - pi/2.0);
			vz = sin((x_percent + s)*angle) * cos((y_percent + t)*angle - pi/2.0);
						
			grid_verts[(x * (GridSize + 1) + y)*3 + 0] = vx*(Radius + TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))]);
			grid_verts[(x * (GridSize + 1) + y)*3 + 1] = vy*(Radius + TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))]);
			grid_verts[(x * (GridSize + 1) + y)*3 + 2] = vz*(Radius + TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))]);
			
			v3 = Vector(0.0, 
						TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))], 
						0.0);
			v2 = Vector(500.0, 
						TempHeight[int(Detail((x+1)%GridSize, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))], 
						0.0);
			v1 = Vector(0.0, 
						TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail((y+1)%GridSize, GridSize/2))], 
						500.0);
						
			n = CalculateNormal(v1, v2, v3);
						
			grid_norms[(x * (GridSize + 1) + y)*3 + 0] += n.x;
			grid_norms[(x * (GridSize + 1) + y)*3 + 1] += n.y;
			grid_norms[(x * (GridSize + 1) + y)*3 + 2] += n.z;
			
			double length = Magnitude(Vector(grid_norms[(x * (GridSize + 1) + y)*3 + 0], grid_norms[(x * (GridSize + 1) + y)*3 + 1], grid_norms[(x * (GridSize + 1) + y)*3 + 2]));
			
			grid_norms[(x * (GridSize + 1) + y)*3 + 0] = (vx/length + grid_norms[(x * (GridSize + 1) + y)*3 + 0])/2.0;
			grid_norms[(x * (GridSize + 1) + y)*3 + 1] = (vy/length + grid_norms[(x * (GridSize + 1) + y)*3 + 0])/2.0;
			grid_norms[(x * (GridSize + 1) + y)*3 + 2] = (vz/length + grid_norms[(x * (GridSize + 1) + y)*3 + 2])/2.0;
			
			grid_norms[(((x+1)%GridSize) * (GridSize + 1) + y)*3 + 0] += n.x;
			grid_norms[(((x+1)%GridSize) * (GridSize + 1) + y)*3 + 1] += n.y;
			grid_norms[(((x+1)%GridSize) * (GridSize + 1) + y)*3 + 2] += n.z;
			
			grid_norms[(x * (GridSize + 1) + ((y+1)%GridSize))*3 + 0] += n.x;
			grid_norms[(x * (GridSize + 1) + ((y+1)%GridSize))*3 + 1] += n.y;
			grid_norms[(x * (GridSize + 1) + ((y+1)%GridSize))*3 + 2] += n.z;
									
			grid_texts[(x * (GridSize + 1) + y)*3 + 0] = 5000*GLfloat(Detail(x, GridSize/2))/GLfloat(Detail(GridSize, GridSize/2));
			grid_texts[(x * (GridSize + 1) + y)*3 + 1] = 5000*GLfloat(Detail(y, GridSize/2))/GLfloat(Detail(GridSize, GridSize/2));
			if (GroundTexture.TextDimension == 2)
				grid_texts[(x * (GridSize + 1) + y)*3 + 2] = 0;
			else
				grid_texts[(x * (GridSize + 1) + y)*3 + 2] = 1.0*max(min(double(TempHeight[int(Detail(x, GridSize/2)*Detail(GridSize+1, GridSize/2) + Detail(y, GridSize/2))] - Min)/(Max - Min), 0.8), 0.2) + 0.0*Temperature;
			
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


void Planet::Render(Player *PlayerPointer)
{
	Physics.Enable();
	
	GLfloat d = Distance(PlayerPointer->PlayerModel.Physics.Position, Physics.Position);
	
	Vector v = Normalize(Vector(cos(grid_x*2.0*pi/WorldRes) * cos(grid_y*pi/WorldRes - pi/2.0), sin(grid_y*pi/WorldRes - pi/2.0), sin(grid_x*2.0*pi/WorldRes) * cos(grid_y*pi/WorldRes - pi/2.0)));
	
	Vector Color = max(Dot(Vector(.97, 0.24, 0.0), v), .1)*Vector(0.0, (pow(10, 1 - (d - Radius)/AtmRad) - 8)/3.333, (pow(10, 1 - (d - Radius)/AtmRad) - 8));
	
	GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat ambi[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};
		
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	
	if (d <= Radius + AtmRad + 1)
		glClearColor(Color.x, Color.y, Color.z, 0.0);
	if (d <= Radius*1.005)
	{
		GLfloat ambi[] = {max(Dot(Vector(.97, 0.24, 0.0), v), 0.0), max(Dot(Vector(.97, 0.24, 0.0), v), 0.0), max(Dot(Vector(.97, 0.24, 0.0), v), 0.0), 1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ambi);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	
		Vector p = Normalize(PlayerPointer->PlayerModel.Physics.Position - Physics.Position);
		
		GLfloat y_angle = asin(p.y) + pi/2.0;
		GLfloat x_angle = acos(p.x/cos(y_angle - pi/2.0));
		
		if (p.x/cos(y_angle - pi/2.0) > 1.0 || p.x/cos(y_angle - pi/2.0) < -1.0)
			x_angle = atan(p.z/p.x);
		
		if (p.z > 0)
			x_angle = -x_angle + 2.0*pi;
		
		if (PlayerPointer->on_ground)
			PlayerPointer->PlayerModel.Physics.Orientation = (180.0/pi)*(Vector(0.0, x_angle, 2.0*pi - acos(Dot(Vector(0.0, 1.0, 0.0), p))));
		
		//x_angle +- acos(d/Radius) and y_angle +- acos(d/Radius)
		
		x_angle /= 2*pi/WorldRes;
		y_angle /= 2*pi/WorldRes;
		x_angle = WorldRes - x_angle;
		
		GLfloat u = x_angle-.5 - grid_x;
		GLfloat v = y_angle-.5 - grid_y;
				
		if (u < 0)
			u *= -1;
		if (v < 0)
			v *= -1;
		
		if (u > WorldRes - .25)
			u -= WorldRes;
		
		if (u >= 0.25 || v >= 0.25)
			LoadGrids(x_angle-.5, y_angle-.5);
		
		GLfloat Length = Magnitude(PlayerPointer->PlayerModel.Physics.Position - Physics.Position) - Radius;
		GLfloat y_pos = (y_angle - GLfloat(grid_y))*GLfloat(Detail(GridSize, GridSize/2.0));
		GLfloat x_pos = (x_angle - GLfloat(grid_x))*GLfloat(Detail(GridSize, GridSize/2.0));
		
		x_pos -= 2000.0*int(x_pos/2000);
		if (x_pos < 0)
			x_pos += 2000.0;
				
		GLfloat t00 = TempHeight[int(x_pos + 0.0)*Detail(GridSize+1, GridSize/2) + int(y_pos + 0.0)],
			    t10 = TempHeight[int(x_pos + 1.0)*Detail(GridSize+1, GridSize/2) + int(y_pos + 0.0)],
				t01 = TempHeight[int(x_pos + 0.0)*Detail(GridSize+1, GridSize/2) + int(y_pos + 1.0)],
				t11 = TempHeight[int(x_pos + 1.0)*Detail(GridSize+1, GridSize/2) + int(y_pos + 1.0)];
		
		GLfloat tx = x_pos - GLfloat(int(x_pos));
		GLfloat ty = y_pos - GLfloat(int(y_pos));

		GLfloat txty = tx * ty;
		
		GLfloat Height =  t00 * (1.0f - ty - tx + txty)
						+ t01 * (tx - txty)
						+ t11 * txty
						+ t10 * (ty - txty) + PlayerPointer->PlayerModel.Radius;
		
		if (Length <= Height)
		{
			PlayerPointer->PlayerModel.Physics.LinearVelocity *= 0.5;
			PlayerPointer->on_ground = true;
			PlayerPointer->PlayerModel.Physics.gravity = true;
			GLfloat x_percent = GLfloat(int(x_pos))/GLfloat(Detail(GridSize, GridSize/2));
			GLfloat y_percent = GLfloat(int(y_pos))/GLfloat(Detail(GridSize, GridSize/2));
			
			Vector v3 = (Radius + t00)*Vector(cos((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
								   sin((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
								   sin((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0));
			
			x_percent = GLfloat(int(x_pos)+1)/GLfloat(Detail(GridSize, GridSize/2));
			y_percent = GLfloat(int(y_pos))/GLfloat(Detail(GridSize, GridSize/2));
			
			Vector v2 = (Radius + t10)*Vector(cos((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
								   sin((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
							       sin((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0));
			
			x_percent = GLfloat(int(x_pos)+1)/GLfloat(Detail(GridSize, GridSize/2));
			y_percent = GLfloat(int(y_pos)+1)/GLfloat(Detail(GridSize, GridSize/2));
			
			Vector v1 = (Radius + t11)*Vector(cos((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
								   sin((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0),
								   sin((x_percent + grid_x)*(2.0*pi/WorldRes)) * cos((y_percent + grid_y)*(2.0*pi/WorldRes) - pi/2.0));
			
			Vector n = CalculateNormal(v1, v2, v3);
			GLfloat D1 = Dot(PlayerPointer->PlayerModel.Physics.Position + MidPoint(PlayerPointer->PlayerModel.Min, PlayerPointer->PlayerModel.Max), n) - Dot(n, v1), D2 = Dot(PlayerPointer->PlayerModel.Physics.Position + MidPoint(PlayerPointer->PlayerModel.Min, PlayerPointer->PlayerModel.Max) + PlayerPointer->PlayerModel.Physics.LinearVelocity, n) - Dot(n, v1);
			GLfloat D;
			if (D1 < 0)
				D = -D1;
			else
				D = D1;
			
			if (Dot(p, n) <= 0)
			{
				Vector v = PlayerPointer->PlayerModel.Physics.LinearVelocity;
				Vector a = PlayerPointer->PlayerModel.Radius*PlayerPointer->PlayerModel.Physics.AngularVelocity;
				
				if (Dot(v, n) < Dot(-1*v, n))
					PlayerPointer->PlayerModel.Physics.LinearVelocity = (v+a - 2*Dot(v+a, n)*n)*PlayerPointer->PlayerModel.Physics.Elasticity;
			}
			else
			{
				if (Magnitude(PlayerPointer->PlayerModel.Physics.LinearVelocity - p*Dot(p, PlayerPointer->PlayerModel.Physics.LinearVelocity)) > .0001)
				{
					if (Dot(PlayerPointer->PlayerModel.Physics.LinearVelocity, n) < Dot(-1*PlayerPointer->PlayerModel.Physics.LinearVelocity, n))
						PlayerPointer->PlayerModel.Physics.LinearVelocity += (D1-D2)*n;
				}
				else
				{
					PlayerPointer->PlayerModel.Physics.gravity = false;
					PlayerPointer->PlayerModel.Physics.LinearVelocity = Vector(0.0, 0.0, 0.0);
				}
				
				if (D <= PlayerPointer->PlayerModel.Radius*.8)
					PlayerPointer->PlayerModel.Physics.Position += n*(PlayerPointer->PlayerModel.Radius*.9 - D);
			}
		}
		else
		{
			PlayerPointer->on_ground = false;
			PlayerPointer->PlayerModel.Physics.gravity = true;
		}
		
		PlayerPointer->GridX = (int)x_angle-(2.0/3.0);
		PlayerPointer->GridY = (int)y_angle-(2.0/3.0);
		PlayerPointer->Planet = 1;
		PlayerPointer->System = 1;
		PlayerPointer->Galaxy = 1;
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, grid_texts);
		glVertexPointer(3, GL_FLOAT, 0, grid_verts);
		glNormalPointer(GL_FLOAT, 0, grid_norms);
		GroundTexture.Enable();
		for (int x = 0; x < GridSize; x++)
			glDrawElements(GL_QUAD_STRIP, (GridSize+1)*2, GL_UNSIGNED_SHORT, &grid_indices[x*(GridSize+1)*2]);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		GroundTexture.Disable();
	}
		
	if (d > Radius*1.005)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, sphere_texts);
		glVertexPointer(3, GL_FLOAT, 0, sphere_verts);
		glNormalPointer(GL_FLOAT, 0, sphere_norms);
		PlanetTexture.Enable();
		for (int x = 0; x < WorldRes; x++)
			glDrawElements(GL_QUAD_STRIP, (WorldRes+1)*2, GL_UNSIGNED_SHORT, &sphere_indices[x*(WorldRes+1)*2]);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		PlanetTexture.Disable();
	}
		
	//glDisable(GL_CULL_FACE);	
	glScalef(1.00094, 1.00094, 1.00094);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, sphere_texts);
	glVertexPointer(3, GL_FLOAT, 0, sphere_verts);
	glNormalPointer(GL_FLOAT, 0, sphere_norms);
	AtmosphereTexture.Enable();
	for (int x = 0; x < WorldRes; x++)
		glDrawElements(GL_QUAD_STRIP, (WorldRes+1)*2, GL_UNSIGNED_SHORT, &sphere_indices[x*(WorldRes+1)*2]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glScalef(1/1.00094, 1/1.00094, 1/1.00094);
	AtmosphereTexture.Disable();
	//glEnable(GL_CULL_FACE);
	
	if (d > Radius*1.005 && d <= Radius + 12000000)
	{
		PlayerPointer->GridX = -1;
		PlayerPointer->GridY = -1;
		PlayerPointer->Planet = 1;
		PlayerPointer->System = 1;
		PlayerPointer->Galaxy = 1;
	}
	
	Physics.Disable();
}

void Planet::Draw(Player *PlayerPointer)
{
	Physics.Enable();
	
	GLfloat d = Distance(PlayerPointer->PlayerModel.Physics.Position, Physics.Position);
	
	if (d <= Radius*1.005)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, grid_texts);
		glVertexPointer(3, GL_FLOAT, 0, grid_verts);
		glNormalPointer(GL_FLOAT, 0, grid_norms);
		for (int x = 0; x < GridSize; x++)
			glDrawElements(GL_QUAD_STRIP, (GridSize+1)*2, GL_UNSIGNED_SHORT, &grid_indices[x*(GridSize+1)*2]);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	Physics.Disable();
}

void Planet::Release()
{
	PlanetTexture.Release();
	AtmosphereTexture.Release();
	GroundTexture.Release();
}
