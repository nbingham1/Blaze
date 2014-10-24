#include "galaxy.h"
#include "../camera.h"

#ifndef universe_h
#define universe_h

struct universehdl
{
	universehdl()
	{
		galaxy_s = NULL;
		galaxy_e = NULL;
	}
	~universehdl()
	{
		galaxy_s = NULL;
		galaxy_e = NULL;
	}

	galaxyhdl *galaxy_s, *galaxy_e;

	octnode_i *ostree;

	double *galaxy_list;
	int num_galaxies;

	int seed;

	camerahdl *camera;
	int cam_address[3]; // galaxy, system, planet

	GLuint cubemap[3];

	void addgalaxy(galaxyhdl *g);
	void remgalaxy(galaxyhdl *g);

	void init(int s, camerahdl *cam);
	bool generate(int n);
	void release();

	void prepare();
	void render();

	vec camerapos();
};

#endif
