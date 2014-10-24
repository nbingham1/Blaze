/*
 *  Star.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 2/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Star.h"

void Star::Load(string name, Camera *r)
{
	Name = name;
	
	int seed = 0;
	for (int x = 0; x < Name.length(); x++)
		seed += int(Name[x]);
	srand(seed);
	Radius = GLdouble(rand()%1000)*1000000.0;
	LightRadius = Radius*GLdouble(rand()%256);
	Physics.Volume = (4.0/3.0)*pi*Radius*Radius*Radius;
	Physics.Mass = 1.0;
	Physics.Inertia = 1.0;
	reference.Initialize(r, &Physics);
	
	int tex_size = 1;
	unsigned char tex[tex_size*tex_size*4];
	
	for (int x = 0; x < tex_size; x++)
	{
		for (int y = 0; y < tex_size; y++)
		{
			tex[(x*tex_size + y)*4 + 0] = 255;
			tex[(x*tex_size + y)*4 + 1] = 170;
			tex[(x*tex_size + y)*4 + 2] = 210;
			tex[(x*tex_size + y)*4 + 3] = 255;
		}
	}
	
	Coloring.Load("res/Texture.vx", "res/Texture.ft");
	
	GLuint Map;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &Map);
	glBindTexture(GL_TEXTURE_2D, Map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_size, tex_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	Coloring.AddTexture(Map);
	
	// Optimization pre-calculations
	RadiusSquared = Radius*Radius;
	LightRadiusSquared = LightRadius*LightRadius;
	BackProjectionDistance = sqrt(LightRadiusSquared + RadiusSquared);
}

void Star::Prepare()
{
	reference.Update();
}

void Star::Render()
{
	GLdouble x_temp, y_temp;
	Vector v;
	
	int size = 20;
		
	GLdouble vert[3*size*size];
	GLdouble text[2*size*size];
	unsigned int indices[size*(size-1)*2];
		
	for (int x = 0; x < size; x++)
	{
		x_temp = 2*pi*(GLdouble(x)/GLdouble(size-1));
		for (int y = 0; y < size; y++)
		{
			y_temp = pi*(GLdouble(y)/GLdouble(size-1)) - pi/2.0;
						
			v.x = cos(x_temp) * cos(y_temp);
			v.y = sin(y_temp);
			v.z = sin(x_temp) * cos(y_temp);
			
			vert[3*(x*size + y) + 0] = v.x*LightRadius - reference.distance*reference.pvec.x;
			vert[3*(x*size + y) + 1] = v.y*LightRadius - reference.distance*reference.pvec.y;
			vert[3*(x*size + y) + 2] = v.z*LightRadius - reference.distance*reference.pvec.z;
			
			text[2*(x*size + y) + 0] = GLdouble(y)/GLdouble(size-1);
			text[2*(x*size + y) + 1] = GLdouble(x)/GLdouble(size-1);
		}
	}
	
	int index = 0;
	for (int x = 1; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			indices[index++] = x*size+y;
			indices[index++] = (x-1)*size+y;
		}
	}
	
	GLdouble d2 = reference.distance*reference.distance;
	
	GLdouble end = (reference.distance < LightRadius ? -1.0*(LightRadius - reference.distance) : sqrt(d2 - LightRadiusSquared));
	GLdouble divisor = (reference.distance < Radius ? sqrt(LightRadiusSquared - RadiusSquared) - end : sqrt(d2 - RadiusSquared) + BackProjectionDistance - end);
	
	glDisable(GL_CULL_FACE);
	Coloring.Use();
	Coloring.ImportNumber(end, "end");
	Coloring.ImportNumber(divisor, "divisor");
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vert);
	glTexCoordPointer(2, GL_DOUBLE, 0, text);
	for (int x = 0; x < size-1; x++)
		glDrawElements(GL_TRIANGLE_STRIP, size*2, GL_UNSIGNED_INT, &indices[x*size*2]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_CULL_FACE);
}

void Star::Release()
{
	Coloring.Release();
}
