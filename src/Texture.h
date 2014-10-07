/*
 *  GameTexture.h
 *  Spiral
 *
 *  Created by Ned Bingham on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "CoreMathematics.h"
#include "OpenGLIncludes.h"

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

void GetDimensions(const char *filename, long *Width, long *Height, long *Depth);
void LoadTexture(const char *filename, long *Width, long *Height, unsigned char *data, int start);
GLuint Load2DTexture(string filename, bool LOD);
GLuint Load3DTexture(string *filename, int Depth, bool LOD);

#endif
