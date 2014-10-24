#include "font.h"

#ifndef text_h
#define text_h

struct texthdl
{
	char name[256];
	char value[512];

	void init(char *n, char *v);
	void render(fonthdl *font);
	void release();
};

#endif
