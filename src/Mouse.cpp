#include "mouse.h"

mousehdl::mousehdl()
{
	speed_mult = 1.0;
}

mousehdl::~mousehdl()
{
}

void mousehdl::init(float speed, int height, int width)
{
	speed_mult = speed;
	sheight = height;
	swidth = width;

	setmouseloc(swidth/2, sheight/2);

	warp = false;
}

void mousehdl::setmouseloc(int x, int y)
{
	glutWarpPointer(x, y);
	warp = true;
}

void mousehdl::setmouseloc(vec v)
{
	glutWarpPointer(v.x, v.y);
	warp = true;
}

vec mousehdl::getdelta(int x, int y)
{
	vec mov(speed_mult*float(y - v)/float(sheight), speed_mult*float(x - h)/float(swidth), 0.0);

	v = y;
	h = x;

	if (!warp)
		return mov;
	else
	{
		warp = false;
		return vec();
	}
}

vec mousehdl::getdelta(vec v)
{
	vec mov(speed_mult*float(v.y - this->v)/float(sheight), speed_mult*float(v.x - h)/float(swidth), 0.0);

	this->v = v.y;
	h = v.x;

	if (!warp)
		return mov;
	else
	{
		warp = false;
		return vec();
	}
}
