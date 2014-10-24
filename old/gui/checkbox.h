#include "../math.h"
#include "font.h"

#ifndef checkbox_h
#define checkbox_h

struct checkboxhdl
{
	string name;
	bool *value;

	void init(const char *n, bool *v);
	void render(fonthdl *font);
	void release();

	void select(vec2i coord);
};

#endif
