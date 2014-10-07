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
void LoadTexture(const char *filename, long *Width, long *Height, rgba_t *T, int start);
GLuint Load2DTexture(string filename, bool LOD);
GLuint Load3DTexture(string *filename, int Depth, bool LOD);

#endif