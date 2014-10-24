#include "display.h"
#include "camera.h"

#include "star.h"
#include "planet.h"

#ifndef renderer_h
#define renderer_h

struct renderhdl
{
	displayhdl display;

	//star s;
	planet p;
	camerahdl  camera;

	void init();
	void release();

	void displayf();
};

#endif
