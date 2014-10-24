#include "../common.h"
#include "../bge_math.h"
#include "font.h"

#ifndef checkbox_h
#define checkbox_h

struct checkboxhdl
{
	char name[256];
	bool *value;

	void init(const char *n, bool *v);
	void render(fonthdl *font);
	void release();

	void select(vec coord);
};

#endif
