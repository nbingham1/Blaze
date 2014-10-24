#include "vector.h"
#include "mathdef.h"
#include "standard.h"
#include "graphics.h"
#include "physics.h"
#include "material.h"
#include "camera.h"
#include "shader.h"

#include "planet.h"

#ifndef star_h
#define star_h

struct star
{
	char name[256];
	int num_planets;
	int seed;

	planet *first, *last;

	physicshdl physics;
	materialhdl material;
	cameraref   camera;

	void init(char *n, camerahdl *cam);
	void release();

	void addplanet(char *name);

	void prepare();
	void render();
};

#endif
