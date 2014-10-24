#include "../standard.h"
#include "../graphics.h"
#include "../mathematics.h"
#include "../camera.h"
#include "../physics.h"

#ifndef asteroid_h
#define asteroid_h

struct asteroidhdl
{
	void init();
	void generate();
	void release();

	void prepare();
	void render();
};

#endif
