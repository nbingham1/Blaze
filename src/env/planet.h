#include "../standard.h"
#include "../graphics.h"
#include "../mathematics.h"
#include "../camera.h"
#include "../physics.h"

#ifndef planet_h
#define planet_h

const int vps = 5;
const int vpn = ((vps*vps + vps)/2);
const int res = 512;

struct systemhdl;
struct planethdl;
struct planet_node;

struct planet_node
{
	double verts[vpn*3];
	float norms[vpn*3];

	double latitude[3]; // calculated as percentage or texture coordinates
	double longatude[3]; // calculated as percentage or texture coordinates

	vec v1, v2, v3;
	vec mid;

	bool cull;

	planet_node *children;
	planet_node *parent;

	planethdl *orgin;

	void init(planethdl *o);
	void generate(vec v1, vec v2, vec v3);
	void release();

	void detail();
	void split();
	void merge();

	void render();
};

struct planethdl
{
	char name[32];

	planet_node data[20];

	physicshdl physics;
	systemhdl *orgin;

	double texts[vpn*2];
	unsigned int indices[vps*vps - 1];
	unsigned char opticaldepth[res];

	double radius;
	double atmradius;

	double seed;

	// 3D array [temperature][himidity][time]
	GLuint *atmosphere;
	int atmosdim[3];

	// 4D array [temperature][precipitation][vegitation][type of texture]
	GLuint *ground;
	int grounddim[4];

	// rg -> vector of technotic plate movement
	// b -> plate edge definition, 255 at edge, 0 elsewhere
	// a -> rock density
	unsigned char *geology;
	int georesolution;

	// r -> temperature from -128 C to 128 C
	// g -> vegitation intensity
	// b -> precipitation
	unsigned char *environment;
	int envresolution;

	// rg -> vector of jet stream movement
	// ba -> vector of wind current movement
	unsigned char *atmospherics;
	int atmresolution;

	bool generate(char *name, double *framerate);
	bool load(char *path);
	void release();

	void prepare();
	void render();

	vec camerapos();
};

#endif
