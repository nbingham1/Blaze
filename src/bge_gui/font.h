/*
 * font.h
 *
 *  Created on: Aug 2, 2011
 *      Author: Ned Bingham
 */

#include "../bge_mtrl.h"
#include "../common.h"

#ifndef font_h
#define font_h

struct fonthdl
{
	fonthdl();
	~fonthdl();

	texture2hdl font_space;
	GLuint font_vx;
	GLuint font_ft;
	GLuint font_pm;

	void initialize(const char *texture, const char *vertex, const char *fragment);
	void release();

	void render();
};

void renderstring2d(fonthdl *f, char string[], float r, float g, float b, float x, float y);
void renderstring3d(fonthdl *f, char string[], float r, float g, float b, float x, float y, float z);

#endif
