/*
 *  OffscreenRenderer.cpp
 *  Infinity Game Engine
 *
 *  Created by Ned Bingham on 12/16/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "OffscreenRenderer.h"

void OffscreenRenderer::Init(int w, int h)
{
	width = w;
	height = h;
	glGenTextures(1, &tex);
}

void OffscreenRenderer::Release()
{
	glDeleteTextures(1, &tex);
}

void OffscreenRenderer::Bind()
{
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

GLuint OffscreenRenderer::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	return tex;
}