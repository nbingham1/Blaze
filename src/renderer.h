#include "display.h"
#include "camera.h"
#include "env/universe.h"

#ifndef renderer_h
#define renderer_h

struct renderhdl
{
	displayhdl display;

	camerahdl  camera;
	universehdl universe;
	galaxyhdl galaxy;

	void init();
	void release();

	void displayf();
};

#endif
