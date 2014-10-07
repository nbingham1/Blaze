/*
 *  World.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 6/19/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "World.h"
#include "Input.h"

void World::Setup(char *filename, GLfloat radius, Player *Pointer)
{
	FILE *file = fopen("res/ptest.raw", "rt");
	heightsize = sqrt(fsize(file));
	heights = new unsigned char[heightsize*heightsize];
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(filename);
		return;
	}
	
	fread(heights, 1, heightsize*heightsize, file);
	
	fclose(file);
	
	file = fopen("res/boundary.raw", "rt");
	roughsize = sqrt(fsize(file));
	roughness = new unsigned char[roughsize*roughsize];
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf("res/boundary.raw");
		return;
	}
	
	fread(roughness, 1, roughsize*roughsize, file);
	
	fclose(file);
	
	LoadMaterials("res/Planet/World.mtl");
	Radius = radius;
	PlayerPointer = Pointer;
	Visual_Angle = 0;
	DispXAngle = 0;
	DispYAngle = 0;
	
	int size = sqrt(size_allotment/8);
	int index = 0;
	for (int x = 1; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			indices[index++] = x*size+y;
			indices[index++] = (x-1)*size+y;
		}
	}
}

void World::LoadMaterials(char *filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	int mat = 0;
	file = fopen(filename, "r");
	char Name[32];
	
	while (!feof(file))
	{
		if (fgets(oneline, 255, file) == NULL)
			break;
		
		if (strncmp(oneline, "newmtl", 6) == 0)
		{
			sscanf(oneline, "newmtl %s", Name);
			Mat.Initialize();
		}
		
		if (strncmp(oneline, "Am", 2) == 0)
		{
			sscanf(oneline, "Am %f %f %f", &v1, &v2, &v3);
			Mat.Ambient.x = v1;
			Mat.Ambient.y = v2;
			Mat.Ambient.z = v3;
		}
		
		if (strncmp(oneline, "Di", 2) == 0)
		{
			sscanf(oneline, "Di %f %f %f", &v1, &v2, &v3);
			Mat.Diffuse.x = v1;
			Mat.Diffuse.y = v2;
			Mat.Diffuse.z = v3;
		}
		
		if (strncmp(oneline, "Sp", 2) == 0)
		{
			sscanf(oneline, "Sp %f %f %f", &v1, &v2, &v3);
			Mat.Specular.x = v1;
			Mat.Specular.y = v2;
			Mat.Specular.z = v3;
		}
		
		if (strncmp(oneline, "Em", 2) == 0)
		{
			sscanf(oneline, "Em %f %f %f", &v1, &v2, &v3);
			Mat.Emission.x = v1;
			Mat.Emission.y = v2;
			Mat.Emission.z = v3;
		}
		
		if (strncmp(oneline, "Op", 2) == 0)
		{
			sscanf(oneline, "Op %f", &v1);
			Mat.Opacity = v1;
		}
		
		if (strncmp(oneline, "Re", 2) == 0)
		{
			sscanf(oneline, "Re %f", &v1);
			Mat.Shininess = v1;
		}
		
		if (strncmp(oneline, "TextMap2D", 9) == 0)
		{
			char LOD[5];
			
			sscanf(oneline, "TextMap2D %s %s", Mat.TextName, LOD);
			if (strncmp(LOD, "LOD", 3) == 0)
				Mat.TextMap = Load2DTexture(string(Mat.TextName), true);
			else
				Mat.TextMap = Load2DTexture(string(Mat.TextName), false);
		}
		
		else if (strncmp(oneline, "TextMap3D", 9) == 0 && mat == 2)
		{
			/*char LOD[5];
						
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
				GroundTexture.TextMap = Load3DTexture(a, TextureDepth, false);*/
		}
			
		if (strncmp(oneline, "DetailMap2D", 11) == 0)
		{
			char LOD[5];
			
			sscanf(oneline, "DetailMap2D %s %s %d", Mat.DetailName, LOD, &Mat.DetailScale);
			if (strncmp(LOD, "LOD", 3) == 0)
				Mat.DetailMap = Load2DTexture(string(Mat.DetailName), true);
			else
				Mat.DetailMap = Load2DTexture(string(Mat.DetailName), false);
		}
	}

	fclose(file);
}

void World::UpdateWorld()
{
	GLfloat Dist = Distance(PlayerPointer->PlayerModel.Physics.Position, Physics.Position);
	GLfloat mult = 1.0;
	if (mult < 1.0)
		mult = 1.0;
	GLfloat vis_angle = acos((Radius-mult*8900.0)/Dist);
	int size = sqrt(size_allotment/8);
	
	Vector player_vec = Normalize(PlayerPointer->PlayerModel.Physics.Position - Physics.Position);
		
	GLfloat y_angle = asin(player_vec.y) + pi/2.0;
	GLfloat x_angle = acos(player_vec.x/cos(y_angle - pi/2.0));
		
	if (player_vec.x/cos(y_angle - pi/2.0) > 1.0 || player_vec.x/cos(y_angle - pi/2.0) < -1.0)
		x_angle = atan(player_vec.z/player_vec.x);
		
	if (player_vec.z > 0)
		x_angle = -x_angle + 2.0*pi;
	
	if (Dist < Radius*1.01)
	{
		PlayerPointer->GridX = (int)x_angle-(2.0/3.0);
		PlayerPointer->GridY = (int)y_angle-(2.0/3.0);
		PlayerPointer->Planet = 1;
		PlayerPointer->System = 1;
		PlayerPointer->Galaxy = 1;
	}
	else if (Dist < Radius*1.5)
	{
		PlayerPointer->GridX = -1;
		PlayerPointer->GridY = -1;
		PlayerPointer->Planet = 1;
		PlayerPointer->System = 1;
		PlayerPointer->Galaxy = 1;
	}
	
	Vector v = Normalize(Vector(cos(x_angle) * cos(y_angle), sin(y_angle), sin(x_angle) * cos(y_angle)));
	
	Vector Color = max(Dot(Vector(.97, 0.24, 0.0), v), .1)*Vector(0.0, (pow(10, 1 - (Dist - Radius)/(.1881*Radius)) - 8)/3.333, (pow(10, 1 - (Dist - Radius)/(.1881*Radius)) - 8));
	
	GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat ambi[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};
		
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	
	if (Dist <= Radius + (.1881*Radius) + 1)
		glClearColor(Color.x, Color.y, Color.z, 0.0);
		
	if (!(vis_angle >= 0.0) && !(vis_angle < 0.0))
		vis_angle =	0.0;
	
	if (absolute(Visual_Angle - vis_angle) >= 2.0*Visual_Angle/double(size) || absolute(DispXAngle - x_angle) > 2.0*Visual_Angle/double(size) || absolute(DispYAngle - y_angle) > 2.0*Visual_Angle/double(size))
	{
		Visual_Angle = vis_angle;
		DispXAngle = x_angle;
		DispYAngle = y_angle;
		Vector v;
		Vector v1, v2, v3, n;
		double x_percent, y_percent;
		GLfloat height = 0;
		GLfloat vis_x = vis_angle, vis_y = vis_angle;
		if (1.0-(2.0*vis_y - vis_y + y_angle)/(pi) > 1.0 || 1.0-(2.0*vis_y - vis_y + y_angle)/(pi) < 0.0)
		{
			vis_x = pi/2.0;
			vis_y = vis_angle + pi/4.0;
		}
		else
		{
			if (absolute(y_angle - pi/2.0) > pi/5.0) 
				vis_x = pi/2.0;
			else
				vis_x = vis_angle;
			vis_y = vis_angle;
		}
		
		for (int x = 0; x < size_allotment; x++)
			data[x] = 0.0;
		
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				x_percent = (1.0-(2.0*(double(x)/(double(size)-1.0))*vis_x - vis_x - x_angle)/(2.0*pi));
				y_percent = (1.0-(2.0*(double(y)/(double(size)-1.0))*vis_y - vis_y + y_angle)/(pi))*.95;
				if (y_percent > 1.0)
				{
					x_percent += .5;
					y_percent = -y_percent + 2.0;
				}
				if (y_percent < 0.0)
				{
					x_percent -= .5;
					y_percent = -y_percent - 2.0;
				}
				
				GLfloat t00 = mult*(((-8900.0*(GLfloat)heights[int(modulate(y_percent, 1.0)*GLfloat(heightsize))*heightsize + int(modulate(x_percent, 1.0)*GLfloat(heightsize))])/256.0)),
						t10 = mult*(((-8900.0*(GLfloat)heights[(int(modulate(y_percent, 1.0)*GLfloat(heightsize)) + 1)*heightsize + int(modulate(x_percent, 1.0)*GLfloat(heightsize))])/256.0)),
						t01 = mult*(((-8900.0*(GLfloat)heights[int(modulate(y_percent, 1.0)*GLfloat(heightsize))*heightsize + (int(modulate(x_percent, 1.0)*GLfloat(heightsize)) + 1)])/256.0)),
						t11 = mult*(((-8900.0*(GLfloat)heights[(int(modulate(y_percent, 1.0)*GLfloat(heightsize)) + 1)*heightsize + (int(modulate(x_percent, 1.0)*GLfloat(heightsize)) + 1)])/256.0));
				
				GLfloat tx = modulate(x_percent, 1.0)*GLfloat(heightsize) - GLfloat(int(modulate(x_percent, 1.0)*GLfloat(heightsize)));
				GLfloat ty = modulate(y_percent, 1.0)*GLfloat(heightsize) - GLfloat(int(modulate(y_percent, 1.0)*GLfloat(heightsize)));
								
				GLfloat txty = tx * ty;
				
				height =  t00 * (1.0f - ty - tx + txty)
								+ t01 * (tx - txty)
								+ t11 * txty
								+ t10 * (ty - txty);
				
				v.x = cos(2.0*(double(x)/(double(size)-1.0))*vis_x - vis_x - x_angle) * cos(2.0*(double(y)/(double(size)-1.0))*vis_y - vis_y + y_angle - pi/2.0);
				v.y = sin(2.0*(double(y)/(double(size)-1.0))*vis_y - vis_y + y_angle - pi/2.0);
				v.z = sin(2.0*(double(x)/(double(size)-1.0))*vis_x - vis_x - x_angle) * cos(2.0*(double(y)/(double(size)-1.0))*vis_y - vis_y + y_angle - pi/2.0);
				
				y_percent = (1.0-(2.0*(double(y)/(double(size)-1.0))*vis_y - vis_y + y_angle)/(pi));
				if (y_percent > 1.0)
					y_percent = -y_percent + 2.0;
				if (y_percent < 0.0)
					y_percent = -y_percent - 2.0;
				
				data[8*(x*size + y) + 0] = x_percent;
				data[8*(x*size + y) + 1] = y_percent;
				data[8*(x*size + y) + 5] = v.x*(Radius + height);
				data[8*(x*size + y) + 6] = v.y*(Radius + height);
				data[8*(x*size + y) + 7] = v.z*(Radius + height);
				if (x != 0 && y != 0)
				{
					v3 = Vector(data[8*(x*size + y) + 5], 
								data[8*(x*size + y) + 6], 
								data[8*(x*size + y) + 7]);
					v2 = Vector(data[8*((x-1)*size + y) + 5], 
								data[8*((x-1)*size + y) + 6], 
								data[8*((x-1)*size + y) + 7]);
					v1 = Vector(data[8*(x*size + (y-1)) + 5], 
								data[8*(x*size + (y-1)) + 6], 
								data[8*(x*size + (y-1)) + 7]);
					
					if (y_percent > 1.0 || y_percent < 0.0)
						n = -1.0*CalculateNormal(v1, v2, v3);
					else
						n = CalculateNormal(v1, v2, v3);
				
					data[8*(x*size + y) + 2] += n.x;
					data[8*(x*size + y) + 3] += n.y;
					data[8*(x*size + y) + 4] += n.z;
					data[8*((x-1)*size + y) + 5] += n.x;
					data[8*((x-1)*size + y) + 6] += n.y;
					data[8*((x-1)*size + y) + 7] += n.z;
					data[8*(x*size + (y-1)) + 5] += n.x;
					data[8*(x*size + (y-1)) + 6] += n.y;
					data[8*(x*size + (y-1)) + 7] += n.z;
				}
				
				if (x != size && y != size)
				{
					v3 = Vector(data[8*(x*size + y) + 5], 
								data[8*(x*size + y) + 6], 
								data[8*(x*size + y) + 7]);
					v2 = Vector(data[8*((x+1)*size + y) + 5], 
								data[8*((x+1)*size + y) + 6], 
								data[8*((x+1)*size + y) + 7]);
					v1 = Vector(data[8*(x*size + (y+1)) + 5], 
								data[8*(x*size + (y+1)) + 6], 
								data[8*(x*size + (y+1)) + 7]);
					
					if (y_percent > 1.0 || y_percent < 0.0)
						n = -1.0*CalculateNormal(v1, v2, v3);
					else
						n = CalculateNormal(v1, v2, v3);
				
					data[8*(x*size + y) + 2] += n.x;
					data[8*(x*size + y) + 3] += n.y;
					data[8*(x*size + y) + 4] += n.z;
					data[8*((x+1)*size + y) + 5] += n.x;
					data[8*((x+1)*size + y) + 6] += n.y;
					data[8*((x+1)*size + y) + 7] += n.z;
					data[8*(x*size + (y+1)) + 5] += n.x;
					data[8*(x*size + (y+1)) + 6] += n.y;
					data[8*(x*size + (y+1)) + 7] += n.z;
				}
			}
		}
	}
}
void World::Render()
{
	Physics.Enable();
	int size = sqrt(size_allotment/8);
	glInterleavedArrays(GL_T2F_N3F_V3F, 0, data);
	Mat.Enable();
	for (int x = 0; x < size; x++)
		glDrawElements(GL_TRIANGLE_STRIP, size*2, GL_UNSIGNED_INT, &indices[x*size*2]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	Mat.Disable();
	Physics.Disable();
}

void World::Release()
{
	delete [] heights;
	delete [] roughness;
	Mat.Release();
}
