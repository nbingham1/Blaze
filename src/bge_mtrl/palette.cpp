/*
 * palette.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#include "palette.h"

palettehdl::palettehdl()
{
	palette_s = NULL;
	palette_e = NULL;
}

palettehdl::~palettehdl()
{
	release();
}

materialhdl *palettehdl::add(const char *directory)
{
	materialhdl *curr = palette_s;
	while (curr != NULL)
	{
		if (strcmp(curr->directory, directory) == 0)
			return curr;

		curr = curr->next;
	}

	materialhdl *m = new materialhdl();
	m->load(directory);
	m->auxf = &palette_s;
	m->auxl = &palette_e;

	if (palette_s == NULL)
	{
		palette_s = m;
		palette_e = m;
	}
	else
	{
		palette_e->auxn = m;
		m->auxp = palette_e;
		palette_e = palette_e->auxn;
	}

	return m;
}

void palettehdl::add(materialhdl **m)
{
	materialhdl *curr = palette_s;
	while (curr != NULL)
	{
		if (strcmp(curr->directory, (*m)->directory) == 0)
		{
			delete (*m);
			*m = curr;

			return;
		}

		curr = curr->next;
	}

	if ((*m) != NULL)
	{
		(*m)->auxf = &palette_s;
		(*m)->auxl = &palette_e;

		if (palette_s == NULL)
		{
			palette_s = (*m);
			palette_e = (*m);
		}
		else
		{
			palette_e->auxn = (*m);
			(*m)->auxp = palette_e;
			palette_e = palette_e->auxn;
		}
	}
}

void palettehdl::remove(materialhdl *m)
{
	if (m != NULL)
	{
		m->release();
		delete m;
	}

	m = NULL;
}

void palettehdl::release()
{
	materialhdl *curr = palette_s, *prev = NULL;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->auxn;
		prev->release();
		delete prev;
		prev = NULL;
	}

	palette_s = NULL;
	palette_e = NULL;
}
