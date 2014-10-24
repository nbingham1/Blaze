#include "../common.h"
#include "../bge_math.h"
#include "value.h"
#include "font.h"

#ifndef slider_h
#define slider_h

struct sliderhdl
{
	char name[256];

	double width;

	double min;
	double max;
	double curr;

	void *value;
	value_t type;

	bool selected;

	void init(char *n, void *v, value_t t, double w, double low, double hi);
	void render(fonthdl *font);
	void release();

	void select(vec coord);
	void unselect();
	void move(vec pos, vec delta);
};

#endif
