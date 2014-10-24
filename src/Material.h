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
#include "Shader.h"
#include "OpenGLIncludes.h"
#include "File.h"

struct Material
{
	char Name[32];
	
	Texture *textures;
	Texture *curr;
	
	GLhandleARB VertShad;
	GLhandleARB FragShad;
	GLhandleARB ShadProg;
	char Vert[255];
	char Frag[255];
	
	
	Material *Next;
	
	void Init();
	void AddTexture(char *name, char *type, int depth);
	void AddTexture(GLuint tex);
	void Load(char *vert, char *frag);
	void Load();
	void Use();
	void Release();
	void ReleaseTextures();
	
	void ImportNumber(GLdouble n, char *name);
	void ImportVector(Vector v, char *name);
};

#endif
