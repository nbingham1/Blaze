#include "../standard.h"
#include "../graphics.h"
#include "../mathematics.h"
#include "../camera.h"
#include "../physics.h"

#ifndef star_h
#define star_h

struct starhdl
{
	char name[32];

	physicshdl physics;

	double radius;

	void init();
	void generate();
	void release();

	void prepare();
	void render();
};

#endif
