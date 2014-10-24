/*
 *  Texture.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/26/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "OpenGLIncludes.h"
#include "CoreMathematics.h"

#ifndef Texture_h
#define Texture_h

struct rgba_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct rgb_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

void GetDimensions(const char *filename, long *Width, long *Height);
void LoadTextData(const char *filename, long *Width, long *Height, rgba_t *T, int start);

struct Texture
{
	char Name[255];
	char Filetype[4];
	int Depth;
	bool LOD;
	GLuint Map;
	
	void SetInfo(char *name, char *type, int depth);
	
	void Load2DTexture(char *filename);
	void Load2DTexture();
	
	void Load3DTexture(char *name, char *type, int depth);
	void Load3DTexture();
	
	void LoadTexture(char *filename, char *type, int depth);
	void LoadTexture();
	
	void Release();
	
	Texture *Next;
};

#endif