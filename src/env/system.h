#include "star.h"
#include "planet.h"
#include "asteroid.h"

#ifndef system_h
#define system_h

struct galaxyhdl;

struct systemhdl
{
	starhdl *star_list;
	int num_stars;

	planethdl *planet_list;
	int num_planets;

	asteroidhdl *asteroid_list;
	int num_asteroids;

	physicshdl physics;
	galaxyhdl *orgin;

	void init(galaxyhdl *o);
	bool generate();
	void release();

	void prepare();
	void render();

	vec camerapos();
};

#endif
