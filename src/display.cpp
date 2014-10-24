/*
 *  display.cpp
 *  Game
 *
 *  Created by Ned Bingham on 9/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "display.h"

void displayhdl::init()
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	timeval gtime;
	gettimeofday(&gtime, NULL);

	starttime = gtime.tv_sec + gtime.tv_usec*1.0E-6;
	currtime = 0.0;
	temptime = 0.0;

	framenum = 0.0;
	framespersecond = 0.0;
	tempframe = 0.0;

	refreshrate_pref = 60.0;
	vsync_pref = false;

	show_framerate = true;

	windowed = false;
}

void displayhdl::renderstring2d(char string[], float r, float g, float b, float x, float y)
{
	glPushMatrix();
	glLoadIdentity();

	glColor3f(r, g, b);

	glRasterPos3f(x*1.6 - .8, y*.985 - .5, -1.0);
	for(unsigned int i = 0; i < strlen(string); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

	glPopMatrix();
}

void displayhdl::renderstring3d(char string[], float r, float g, float b, float x, float y, float z)
{
	glColor3f(r, g, b);

	glRasterPos3f(x, y, z);
	for(unsigned int i = 0; i < strlen(string); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}

bool displayhdl::vsync()
{
	double t = gettime() - trunc(gettime());
	t = t*refreshrate_pref - trunc(t*refreshrate_pref);
	t = t < 0.0 ? -t : t;

	if (t < 0.01 || !vsync_pref)
		return true;
	return false;
}

double displayhdl::gettime()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	currtime = gtime.tv_sec + gtime.tv_usec*1.0E-6 - starttime;
	if (currtime > 1200.0)
		exit(1);
	return currtime;
}

double displayhdl::getfps()
{
	gettime();
	if (int(currtime*10.0) != int(temptime*10.0))
	{
		framespersecond = (framenum - tempframe)/(currtime - temptime);
		temptime = currtime;
		tempframe = framenum;
	}

	return framespersecond;
}

void displayhdl::render()
{
	glUseProgram(0);
	char str[256];
	framenum++;
	if (show_framerate)
	{
		sprintf(str, "%f seconds", gettime());
		renderstring2d(str, 1.0, 1.0, 1.0, 0.0, 1.0);
		sprintf(str, "%f frames", framenum);
		renderstring2d(str, 1.0, 1.0, 1.0, 0.0, .98);
		sprintf(str, "%f fps ave", framenum/currtime);
		renderstring2d(str, 1.0, 1.0, 1.0, 0.0, .96);
		sprintf(str, "%f fps", getfps());
		renderstring2d(str, 1.0, 1.0, 1.0, 0.0, .94);
	}
}
