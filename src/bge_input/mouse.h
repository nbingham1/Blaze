#include "../bge_math.h"

#ifndef mouse_h
#define mouse_h

struct mousehdl
{
	mousehdl();
	~mousehdl();

	int h;
	int v;
	double d;
	bool warp;
	bool bind;

	int sheight;
	int swidth;

	float speed_mult;

	bool buttons[32];

	void init(float speed, int height, int width);

	void setmouseloc(int x, int y);
	void setmouseloc(vec v);

	vec getdelta(int x, int y);
	vec getdelta(vec v);

	void switchbind();

	void mouse_down(int button);
	void mouse_up(int button);

	void getdepth();
};

#endif
