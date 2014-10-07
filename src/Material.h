/*
 *  Material.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 12/2/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Material_h
#define Material_h

#include "Texture.h"
#include "OpenGLIncludes.h"

struct Material
{
	char Name[32];
	
	char TextName[255];
	char DetailName[255];
	
	Vector Ambient;
	Vector Diffuse;
	Vector Specular;
	Vector Emission;
	
	GLdouble Shininess;
	GLdouble Opacity;
	
	GLuint TextMap;
	GLuint DetailMap;
	
	int TextDimension;
	int DetailDimension;
	int DetailScale;
		
	Material *Next;
	
	void Initialize();
	void Enable(GLdouble *texts);
	void Enable();
	void Disable();
	void Release();
};

#endif