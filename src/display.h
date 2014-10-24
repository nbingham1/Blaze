#include "graphics.h"
#include "standard.h"

#ifndef display_h
#define display_h

struct displayhdl
{
	int width;
	int height;

	double currtime;
	double starttime;
	double temptime;

	double framenum;
	double framespersecond;
	double tempframe;

	double refreshrate_pref;
	bool vsync_pref;

	bool show_framerate;

	bool windowed;

	void init();
	void renderstring2d(char string[], float r, float g, float b, float x, float y);
	void renderstring3d(char string[], float r, float g, float b, float x, float y, float z);

	bool vsync();

	double gettime();
	double getfps();

	void render();
};

#endif
