/*
	GameParticle.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 7/14/06.
	Copyright 2006 Sol Union. All rights reserved.

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

#include "GameParticle.h"
#include "GameApp.h"
extern GameApp App;

void ParticleEmitter::SetArea(Vector min, Vector max)
{
	Min = min;
	Max = max;
}

void ParticleEmitter::Explode(int Num, GLfloat force, Vector min, Vector max, bool constant)
{
	if (!constant)
	{
		Vector Velocity;

		Min = min;
		Max = max;
		for (int x = 0; x < Num; x++)
		{
			Velocity = Vector(GLfloat(rand() % 101 - 50)/100, GLfloat(rand() % 101 - 50)/100, GLfloat(rand() % 101 - 50)/100);
			Velocity = Normalize(Velocity)*(force/(rand() % (int)force + 1));
			
			CreateParticle(Velocity.x, Velocity.y, Velocity.z);
		}
	}
	else
	{
		NumberEmittedPerFrame = Num;
		Min = min;
		Max = max;
		Force = force;
	}
}

void ParticleEmitter::ConstantEmit(int Num, Vector Force, Vector min, Vector max)
{
	Forces += Force;
	Min = min;
	Max = max;
	NumberEmittedPerFrame = Num;
}

void ParticleEmitter::Emit(int Num, Vector Min, Vector Max, Vector Force, GLfloat f)
{
	if (Explosion)
		Explode(Num, f, Min, Max, ConstantEmission);
	else
		ConstantEmit(Num, Force, Min, Max);
	NumLiveParticles++;
}

void ParticleEmitter::StopEmit()
{
	NumberEmittedPerFrame = 0;
}

void ParticleEmitter::Render()
{
	glDisable(GL_FOG);
	if (ConstantEmission)
	{
		if (Explosion)
		{
			Vector Velocity;
			for (int x = 0; x < NumberEmittedPerFrame; x++)
			{
				Velocity = Vector(GLfloat(rand() % 101 - 50)/100, GLfloat(rand() % 101 - 50)/100, GLfloat(rand() % 101 - 50)/100);
				Velocity = Normalize(Velocity)*(Force/(rand() % int(Force) + 1));
				
				CreateParticle(Velocity.x, Velocity.y, Velocity.z);
			}
		}
		else
			for (int x = 0; x < NumberEmittedPerFrame; x++)
				CreateParticle(Forces.x, Forces.y, Forces.z);
	}
	
	Vector Momentum;
	NumLiveParticles = 0;
	for (int x = 0; x < max_particles; x++)
	{
		p[x].life--;
		if (p[x].life > 0.0)
		{
			NumLiveParticles++;
			Momentum = p[x].LinearVelocity*mass;
			p[x].Position += Momentum;
			
			p[x].LinearVelocity += Forces;
		}
	}
	
	Vector TopRight, TopLeft, BottomRight, BottomLeft;
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Vector Up = Vector(m[0], m[4], m[8]);
	Vector Right;
	if (Direct)
		Right = Vector(-m[1], m[5], -m[9]);
	else
		Right = Vector(0.0, 2.0, 0.0);
	GLfloat f[4];
	
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	for (int x = 0; x < max_particles; x++)
	{
		if (p[x].life > 0.0)
		{
			TopRight = Vector(-1, 1, -1)*(Right+Up)*(size) + p[x].Position;
			TopLeft = Vector(-1, 1, -1)*(Up-Right)*(size) + p[x].Position;
			BottomRight = Vector(-1, 1, -1)*(Right-Up)*(size) + p[x].Position;
			BottomLeft = Vector(1, -1, 1)*(Right+Up)*(size) + p[x].Position;
			
			f[0] = 1.0;
			f[1] = 1.0;
			f[2] = 1.0;
			f[3] = p[x].life/life;
			glColor4fv(f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(TopRight.x, TopRight.y, TopRight.z);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(BottomRight.x, BottomRight.y, BottomRight.z);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(BottomLeft.x, BottomLeft.y, BottomLeft.z);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(TopLeft.x, TopLeft.y, TopLeft.z);
		}
	}
	glEnd();
	glEnable(GL_FOG);
}

void ParticleEmitter::LoadFromFile(string filename)
{
	FILE *file;
	char oneline[255];
	GLfloat v1, v2, v3;
	
	char texname[255];
	
	ConstantEmission = false;
	Explosion = false;
	
	file = fopen(string_to_char(filename), "rt");
	if (file == NULL)
	{
		printf("error, file not found!\n");
		printf(string_to_char(filename));
		printf("\n");
		exit(2);
	}
	
	while (!feof(file))
	{
		if (better_fgets(oneline, 255, file) == NULL)
			break;
				
		if (strncmp(oneline, "Life", 4) == 0)
		{
			sscanf(oneline, "Life %f", &v1);
			life = v1;
		}
		
		if (strncmp(oneline, "Mass", 4) == 0)
		{
			sscanf(oneline, "Mass %f", &v1);
			mass = v1;
		}
		
		if (strncmp(oneline, "Size", 4) == 0)
		{
			sscanf(oneline, "Size %f", &v1);
			size = v1;
		}
		
		if (strncmp(oneline, "Force", 5) == 0)
		{
			sscanf(oneline, "Force %f %f %f", &v1, &v2, &v3);
			Forces.x = v1;
			Forces.y = v2;
			Forces.z = v3;
		}
		
		if (strncmp(oneline, "ConstantEmission", 16) == 0)
			ConstantEmission = true;
		if (strncmp(oneline, "Explosion", 9) == 0)
			Explosion = true;
		if (strncmp(oneline, "Direct", 6) == 0)
			Direct = true;
		if (strncmp(oneline, "TexName", 7) == 0)
			sscanf(oneline, "TexName %s", texname);
	}
	
	tex = Load2DTexture(string(texname), false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	fclose(file);
}

void ParticleEngine::Init(int Num, string *filenames)
{
	NumEmitters = Num;
	Emitters = new ParticleEmitter[Num];
	for (int x = 0; x < Num; x++)
	{
		Emitters[x].LoadFromFile(filenames[x]);
	}
}

void ParticleEngine::Release()
{
	//for (int x = 0; x < NumEmitters; x++)
	//	glDeleteTextures(1, &Emitters[x].tex);
	if (Emitters)
	{
		delete [] Emitters;
		Emitters = NULL;
	}
}

void ParticleEngine::Render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	for (int x = 0; x < NumEmitters; x++)
		if (Emitters[x].NumLiveParticles > 0)
			Emitters[x].Render();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void ParticleEngine::Rain(int intensity, Vector Min, Vector Max)
{
	Emitters[0].Emit(intensity, Min, Max, Vector(0.0, -0.5, 0.0), 0);
}

void ParticleEngine::Snow(int intensity, Vector Min, Vector Max)
{
	Emitters[1].Emit(intensity, Min, Max, Vector(0.0, 0.0, 0.0), 0);
}

void ParticleEngine::ExplodeFire(int intensity, Vector Position)
{
	Emitters[2].Emit(intensity, Position, Position, Vector(0.0, 0.0, 0.0), intensity);
}

void ParticleEngine::ExplodeDirt(int intensity, Vector Position)
{
	Emitters[3].Emit(intensity, Position, Position, Vector(0.0, 0.0, 0.0), intensity);
}

void ParticleEngine::ExplodeGrass(int intensity, Vector Position)
{
	Emitters[4].Emit(intensity, Position, Position, Vector(0.0, 0.0, 0.0), intensity);
}

void ParticleEngine::Spark(int intensity, Vector Position)
{
	Emitters[5].Emit(intensity, Position, Position, Vector(0.0, 0.0, 0.0), intensity);
}
