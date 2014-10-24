#include "../bge_math.h"
#include "../bge_mtrl.h"
#include "../common.h"

#include "object.h"

#ifndef planet_h
#define planet_h

#define OBJ_PLANET 2

const int verts_per_side = 21;
const int verts_per_part = (verts_per_side*verts_per_side + verts_per_side)/2;
const int pixels_per_side = 256;
const int pixels_per_part = pixels_per_side*pixels_per_side;
const int indices_per_part = verts_per_side*verts_per_side;

const int max_parts   = 2001;
const int max_verts   = verts_per_part*(max_parts+1);
const int max_indices = indices_per_part*(max_parts+1) - 1;

const int min_radius = 1000;
const int max_radius = 100000000;

const int num_textures = 5;
const int tex_size = 512;

struct bufferprt;
struct planetprt;
struct planethdl;

struct bufferprt
{
	bufferprt(int off);
	~bufferprt();

	planetprt *part;
	int offset;				// the position of the geometry
							// data in the indices array by indice

	bufferprt *next;
	bufferprt *prev;
};

struct planetprt
{
	planetprt();
	~planetprt();

	location center;		// the center of the triangle
	location v1, v2, v3;	// the corners of the triangle
	vec nv1, nv2, nv3, ncenter;		// normalized direction vectors to the corners of the triangle
	vec ntangent;
	double a1, a2, a3;		// the angles at the corners
	double radius;			// the angle from the center to a vertice

	double min, max;		// min height and max height

	int level;				// the split level
	int polarity;			// the middle node is inversed
	float interpolator;     // for geomorphing

	bool cull;				// whether or not to cull this part
	bool update_split;		// whether or not this node needs to update the vertex buffer after a split
	bool update_merge;		// whether or not this node needs to update the vertex buffer after a merge

	planethdl *orgin;

	// tree structure
	planetprt *parent;
	planetprt *children;
	int sibling_number;
	planetprt *neighbors[3]; // this points to the neighbors of this node at levels up to the level of this node (closest to the leaf node)
	int neighbor_type[3]; // v1 - v1 & v2 - v2 or v1 - v2 and v2 - v1 determines positive or negative, and the relation determines 1 2 or 3

	bufferprt *buffer_location;

	void initialize(planethdl *org, location vert1, location vert2, location vert3, planetprt *n1, int nt1, planetprt *n2, int nt2, planetprt *n3, int nt3);
	void getheight(location vc, double *h);
	void findneighbor(int relation);
	void update_neighbors();

	void prepare();
	void render(GLuint program);
	void release();
};

struct planethdl : objecthdl
{
private:
	virtual void initialize_derived();
	virtual void release_derived();

	virtual void prepare_derived();
	virtual void render_derived();

public:
	planethdl();
	planethdl(unsigned int s);
	~planethdl();

	planetprt part_list[20];
	noise3hdl noise;
	unsigned int seed;

	bufferprt *first_leaf, *last_leaf;
	bufferprt *first_unused, *last_unused;

	int num_parts;

	float height_multiplier;
	float noise_density;

	// Terrain Data
	vertex_v3n3t3g3 terrain_geometry[max_verts];
	GLuint terrain_geo;
	GLuint terrain_ind;

	materialhdl *material;

	textureahdl	terrain_tx;							// rgb -> a grid of different textures for this planet
													// a -> bump maps
	GLuint terrain_vx;
	GLuint terrain_ft;
	GLuint terrain_pm;

	// Atmosphere Shell Data
	double atmosphere_radius;
	int atmosphere_size;

	texture3hdl atmosphere_tx;

	GLuint atmosphere_geo;
	GLuint atmosphere_ind;

	GLuint atmosphere_vx;
	GLuint atmosphere_ft;
	GLuint atmosphere_pm;

	// Ocean Shell Data
	GLuint ocean_vx;
	GLuint ocean_ft;
	GLuint ocean_pm;

	float index_refraction;
	float air_density;

	// Camera Management
	vec camera_direction;
	double camera_distance;

	void render_terrain();
	void render_ocean();
	void render_atmosphere();

	void allocate_part(planetprt *part);
	void deallocate_part(planetprt *part);

	void split(planetprt *part);
	void merge(planetprt *part);

	noisevec terrain(location n);

	// Debug GUI
	virtual void generate_info(containerhdl *contain);
	virtual void generate_menu(containerhdl *contain);
};

#endif
