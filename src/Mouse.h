#include "graphics.h"
#include "vector.h"

#ifndef mouse_h
#define mouse_h

struct mousehdl
{
	mousehdl();
	~mousehdl();

	int h;
	int v;
	bool warp;

	int sheight;
	int swidth;

	float speed_mult;

	void init(float speed, int height, int width);

	void setmouseloc(int x, int y);
	void setmouseloc(vec v);

	vec getdelta(int x, int y);
	vec getdelta(vec v);

};

#endif
