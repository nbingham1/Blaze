#include "vector.h"
#include "mathdef.h"
#include "standard.h"
#include "graphics.h"
#include "physics.h"
#include "material.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

#ifndef planet_h
#define planet_h

const int vps = 7;
const int vpn = ((vps*vps + vps)/2);

struct planet;
struct planet_node;

struct planet_node
{
	int seed;

	double *verts;
	double *data;
	double *ldata;

	planet_node *children;
	planet_node *parent;
	planet      *orgin;

	bool cull;
	bool cullliq;

	vec v1, v2, v3;
	vec mid;

	double size;
	double min, max;

	int splitdepth;

	void init(planet *o);
	void generate(vec vec1, vec vec2, vec vec3);
	void detail();
	void split();
	void merge();
	void release();

	void render();
	void render_liquid();

	void getheight(vec vc, double *h);
};

struct planet
{
	char name[256];

	int seed;

	double atmos_radius;

	planet_node data[20];

	double texts[vpn*2];
	unsigned int indices[vps*vps - 1];

	unsigned char *atm_optdepth;
	int atm_optdepthsize;

	physicshdl  physics;
	materialhdl ground_mat;
	materialhdl liquid_mat;
	materialhdl atmos_mat;
	cameraref   camera;

	planet *next, *prev;

	void init(char *n, camerahdl *cam);
	void release();

	void prepare();
	void render();
};

#endif
