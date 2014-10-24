#include "mouse.h"
#include <stdlib.h>
#include <stdio.h>

mousehdl::mousehdl()
{
	warp = false;
	h = 0;
	v = 0;
	swidth = 0;
	sheight = 0;
	speed_mult = 0;
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
	h = x;
	v = y;
	glutWarpPointer(x, y);
	warp = true;
}

void mousehdl::setmouseloc(Vector v)
{
	h = (int)v.x;
	this->v = (int)v.y;
	glutWarpPointer((int)v.x, (int)v.y);
	warp = true;
}

Vector mousehdl::getdelta(int x, int y)
{
	Vector mov(speed_mult*GLdouble(y - v)/GLdouble(sheight), speed_mult*GLdouble(x - h)/GLdouble(swidth), 0.0);

	v = y;
	h = x;
	if (!warp)
		return mov;
	else
	{
		warp = false;
		return Vector();
	}
}

Vector mousehdl::getdelta(Vector v)
{
	Vector mov(speed_mult*GLdouble(v.y - this->v)/GLdouble(sheight), speed_mult*GLdouble(v.x - h)/GLdouble(swidth), 0.0);

	this->v = v.y;
	h = v.x;

	return mov;
}
