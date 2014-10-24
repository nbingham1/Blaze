/*
 * planet.h
 *
 *  Created on: Nov 26, 2013
 *      Author: nbingham
 */

#include "../base.h"
#include "../graphics.h"
#include "object.h"

#ifndef planet_h
#define planet_h

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

struct render_vertex
{
	float v[3];
	float n[3];
	float t[3];

	float g[2];
	float dn[3];

	void copy(render_vertex v);
	void copyshift(render_vertex v, float *voff);
} __attribute__ ((packed));

struct planet_buffer;
struct planet_part;
struct planethdl;

struct planet_buffer : ilist_node<planet_buffer>
{
	planet_buffer(int off);
	~planet_buffer();

	planet_part *part;
	int offset;			// the position of the geometry data in the indices array by index
};

struct planet_part : iatree<planet_part>
{
	planet_part();
	~planet_part();

	vec3F center;							// the center of the triangle
	vec3F v1, v2, v3;						// the corners of the triangle
	vec3f nv1, nv2, nv3, ncenter, ntangent; // normalized direction vectors to the corners of the triangle
	double a1, a2, a3;						// the angles at the corners
	double radius;							// the distance from the center to a vertex
	double min, max;						// min height and max height

	int level;								// the split level
	int polarity;							// the middle node is inverted
	float interpolator;						// for geomorphing

	bool cull;								// whether or not to cull this part
	bool update_split;		// whether or not this node needs to update the vertex buffer after a split
	bool update_merge;		// whether or not this node needs to update the vertex buffer after a merge


	planethdl *origin;
	int sibling_number;
	planet_part *neighbors[3];				// this points to the neighbors of this node at levels up to the level of this node (closest to the leaf node)
	int neighbor_type[3];					// v1 - v1 & v2 - v2 or v1 - v2 and v2 - v1 determines positive or negative, and the relation determines 1 2 or 3

	planet_buffer *buffer;

	void initialize(planethdl *org, vec3F vert1, vec3F vert2, vec3F vert3, planet_part *n1, int nt1, planet_part *n2, int nt2, planet_part *n3, int nt3);
	void findneighbor(int relation);
	void update_neighbors();

	void prepare();
	void render(GLuint program);
	void release();
};

struct planethdl : objecthdl
{
	poly base;
	double boundary;

	planet_part parts[20];

	ilist<planet_buffer> used;
	ilist<planet_buffer> unused;

	render_vertex geometry[max_verts];
	GLuint geometry_vbo;
	GLuint geometry_ind;

	materialhdl *land;

	vec3f camera_direction;
	double camera_distance;

	void allocate_part(planet_part *part);
	void deallocate_part(planet_part *part);

	void initialize(poly base, double boundary);
	void split(planet_part *part);
	void merge(planet_part *part);
};

#endif
