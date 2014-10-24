/*
 * material.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "material.h"

materialhdl::materialhdl()
{
	vertex = 0;
	fragment = 0;
	program = 0;

	pthread_rwlock_init(&lock, NULL);

	first = NULL;
	last = NULL;
	next = NULL;
	prev = NULL;

	auxf = NULL;
	auxl = NULL;
	auxn = NULL;
	auxp = NULL;
}

materialhdl::~materialhdl()
{
	release();
}

void materialhdl::swrite()
{
	pthread_rwlock_wrlock(&lock);
}

void materialhdl::ewrite()
{
	pthread_rwlock_unlock(&lock);
}

void materialhdl::sread()
{
	pthread_rwlock_rdlock(&lock);
}

void materialhdl::eread()
{
	pthread_rwlock_unlock(&lock);
}

void materialhdl::load(const char *directory)
{
	swrite();

	int TextLen;
	int w = 0, h = 0;
	char ErrText[1024];

	DIR *dir = opendir(directory);
	dirent *ent;
	if (dir == NULL)
	{
		printf("Error: could not open %s.\n", directory);
		printf("\tOccurred on line %d in file %s.\n", __LINE__, __FILE__);
		return;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		if (sscanf(ent->d_name, "%*s.vx"))
		{
			TextLen = 1024;
			vertex = LoadGLSLShader(GL_VERTEX_SHADER, ent->d_name, ErrText, &TextLen);
			printf("%s", ErrText);
		}
		else if (sscanf(ent->d_name, "%*s.ft"))
		{
			TextLen = 1024;
			fragment = LoadGLSLShader(GL_FRAGMENT_SHADER, ent->d_name, ErrText, &TextLen);
			printf("%s", ErrText);
		}
		else if (sscanf(ent->d_name, "%*s.tga"))
		{
			rgb_tga(ent->d_name, &w, &h);
		}
		else
		{

		}
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	ewrite();
}

void materialhdl::render()
{
	sread();



	eread();
}

void materialhdl::release()
{
	swrite();

	pthread_rwlock_destroy(&lock);

	if (auxn)
		auxn->auxp = auxp;
	if (auxp)
		auxp->auxn = auxn;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	if (*auxf == this)
		auxf = &(this->auxn);
	if (*auxl == this)
		auxl = &(this->auxp);
	if (*first == this)
		first = &(this->next);
	if (*last == this)
		last = &(this->prev);

	texture.release();

	if (vertex != 0)
		glDeleteShader(vertex);
	if (fragment != 0)
		glDeleteShader(fragment);
	if (program != 0)
		glDeleteShader(program);

	vertex = 0;
	fragment = 0;
	program = 0;

	first = NULL;
	last = NULL;
	next = NULL;
	prev = NULL;

	auxf = NULL;
	auxl = NULL;
	auxn = NULL;
	auxp = NULL;
}
