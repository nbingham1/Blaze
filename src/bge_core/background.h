#include "../common.h"

#ifndef background_h
#define background_h

struct backgroundhdl
{
	backgroundhdl();
	~backgroundhdl();

	GLuint geometry_object;
	GLuint indices_object;

	GLuint background_vx;
	GLuint background_ft;
	GLuint background_pm;

	GLuint background_tx;

	void initialize();
	void render();
	void release();
};

#endif
