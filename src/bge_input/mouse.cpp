#include "mouse.h"
#include "../common.h"

mousehdl::mousehdl()
{
	speed_mult = 1.0;
	bind = true;
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
	bind = true;
	glutSetCursor(GLUT_CURSOR_NONE);

	for (int x = 0; x < 32; x++)
		buttons[x] = false;
}

void mousehdl::setmouseloc(int x, int y)
{
	glutWarpPointer(x, y);
	warp = true;
}

void mousehdl::setmouseloc(vec v)
{
	glutWarpPointer((int)v.x, (int)v.y);
	warp = true;
}

vec mousehdl::getdelta(int x, int y)
{
	vec mov(float(x - h), float(v - y), 0.0);

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
	vec mov(float(v.x - h), float(this->v - v.y), 0.0);

	this->v = (int)v.y;
	h = (int)v.x;

	if (!warp)
		return mov;
	else
	{
		warp = false;
		return vec();
	}
}

void mousehdl::switchbind()
{
	bind = 1-bind;
	if (bind)
	{
		glutSetCursor(GLUT_CURSOR_NONE);
		setmouseloc(vec(720.0, 450.0));
	}
	else
	{
		setmouseloc(vec(720.0, 450.0));
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
}

void mousehdl::mouse_down(int button)
{
	buttons[button] = true;
}

void mousehdl::mouse_up(int button)
{
	buttons[button] = false;
}

void mousehdl::getdepth()
{
	unsigned char c;
	glReadPixels(h, sheight - v, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &c);
	d = double(c)/255.0;
}
