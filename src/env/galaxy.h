#include "system.h"
#include "nebula.h"

#ifndef galaxy_h
#define galaxy_h

struct universehdl;

struct galaxyhdl
{
	galaxyhdl()
	{
		next = NULL;
		prev = NULL;
	}
	~galaxyhdl()
	{
		next = NULL;
		prev = NULL;
	}

	int seed;

	systemhdl *system;
	nebulahdl *nebula;
	octnode_i *ostree;

	double *system_list;
	int num_systems;
	double *nebula_list;
	int num_nebula;

	physicshdl physics;
	universehdl *orgin;

	galaxyhdl *next, *prev;

	void init(universehdl *o, int s);
	void *generate(void *buf);
	void release();

	void prepare();
	void render();

	vec camerapos();
};

#endif
