#include "graphics.h"
#include "Vector.h"

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
	void setmouseloc(Vector v);

	Vector getdelta(int x, int y);
	Vector getdelta(Vector v);

};

#endif
