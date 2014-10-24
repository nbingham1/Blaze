/*
 *  OffscreenRenderer.h
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 12/16/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "graphics.h"

#ifndef OffscreenRenderer_h
#define OffscreenRenderer_h

struct OffscreenRenderer
{
	GLuint tex;
	GLint viewport[4];
	int width, height;

	void Init(int w, int h);
	void Release();

	void Bind();
	GLuint Unbind();
};

#endif
