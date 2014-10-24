#include "../standard.h"
#include "../graphics.h"
#include "../mathematics.h"
#include "../camera.h"
#include "../physics.h"

#ifndef nebula_h
#define nebula_h

struct galaxyhdl;

struct nebulahdl
{
	galaxyhdl *orgin;

	void init(galaxyhdl *o);
	bool generate();
	void release();

	void prepare();
	void render(bool expand);
};

#endif
