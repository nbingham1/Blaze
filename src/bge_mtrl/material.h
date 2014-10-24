/*
 * material.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "texture.h"
#include "shader.h"
#include "../common.h"

#ifndef material_h
#define material_h

struct materialhdl
{
	materialhdl();
	~materialhdl();

	char directory[256];

	texturehdl texture;

	GLuint vertex;
	GLuint fragment;
	GLuint program;

	pthread_rwlock_t lock;

	materialhdl **first, **last;
	materialhdl *next, *prev;

	materialhdl **auxf, **auxl;
	materialhdl *auxn, *auxp;

	void swrite();
	void ewrite();

	void sread();
	void eread();

	void load(const char *directory);
	void render();
	void release();
};

#endif
