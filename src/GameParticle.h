/*
	GameParticle.h
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

#include "GeometricalTypes.h"
#include "GameTexture.h"

#ifndef GameParticle_h
#define GameParticle_h

#define max_particles 20000

struct Particle
{
	Particle()
	{
		life = -1.0;
		Position = Vector();
		LinearVelocity = Vector();
	}
	~Particle()
	{
	}

	Vector Position;
	Vector LinearVelocity;
		
	GLfloat life;
};

struct ParticleEmitter
{
	ParticleEmitter()
	{
		tex = 0;
		NumLiveParticles = 0;
		ConstantEmission = false;
		Explosion = false;
		Direct = false;
		NumberEmittedPerFrame = 0;

		life = -1.0;
		mass = 1.0;
		size = 1.0;
		Force = 0.0;
	}
	~ParticleEmitter()
	{
	}

	GLuint tex;
	
	Particle p[max_particles];
	int NumLiveParticles;
	
	bool ConstantEmission;
	bool Explosion;
	bool Direct;
	int NumberEmittedPerFrame;
	
	GLfloat Force;
	Vector Forces;
	
	Vector Min, Max;
	
	GLfloat life;
	GLfloat mass;
	GLfloat size;
	
	inline void CreateParticle(GLfloat x, GLfloat y, GLfloat z)
	{
		if (NumLiveParticles < max_particles)
		{
			int pnum = 0;
			for (int X = 0; X < max_particles; X++)
			{
				if (p[X].life <= 0)
				{
					pnum = X;
					break;
				}
			}
			
			p[pnum].life = life;
			p[pnum].Position = Vector(rand() % (int)(Max.x+1 - Min.x) + Min.x, rand() % (int)(Max.y+1 - Min.y) + Min.y, rand() % (int)(Max.z+1 - Min.z) + Min.z);
			p[pnum].LinearVelocity = Vector(x, y, z);
			NumLiveParticles++;
		}
	}
	
	void SetArea(Vector min, Vector max);
	
	void Explode(int Num, GLfloat force, Vector min, Vector max, bool constant);
	void ConstantEmit(int Num, Vector Force, Vector min, Vector max);
	void Emit(int Num, Vector Force, Vector Min, Vector Max, GLfloat f);
	void StopEmit();
	void Render();
	void LoadFromFile(string filename);
};

struct ParticleEngine
{
	ParticleEngine()
	{
		Emitters = NULL;
		NumEmitters = 0;
	}
	~ParticleEngine()
	{
		Release();
	}
	ParticleEmitter *Emitters;
	int NumEmitters;
	
	void Init(int Num, string *filenames);
	void Render();
	void Release();
	
	void Rain(int intensity, Vector Min, Vector Max);
	void Snow(int intensity, Vector Min, Vector Max);
	void ExplodeFire(int intensity, Vector Position);
	void ExplodeDirt(int intensity, Vector Position);
	void ExplodeGrass(int intensity, Vector Position);
	void Spark(int intensity, Vector Position);
};

#endif
