/*
 * block.h
 *
 *  Created on: May 23, 2015
 *      Author: nbingham
 */

#include <math/geometry.h>
#include "graphics/opengl.h"
#include <pthread.h>

using namespace core;

#ifndef block_h
#define block_h

#define cubes_per_side 64

struct rw_lock
{
	rw_lock();
	~rw_lock();

	pthread_mutex_t lock;
	pthread_cond_t  reader_gate;
	pthread_cond_t  writer_gate;
	int count;
	int wait;

	bool w_lock();
	bool w_trylock();
	bool w_unlock();
	bool r_lock();
	bool r_trylock();
	bool r_unlock();
};

struct blockhdl
{
	blockhdl();
	blockhdl(vec3F origin, double size, grad3f (*density)(gvec3f location, void *data) = NULL);
	~blockhdl();

	grad3f (*density)(gvec3f location, void *data);
	array<grad3f> values;
	array<int> grid;
	vec3F origin;
	float size;

	rw_lock geo_lock;
	array<vec3f> points;
	array<int> indices;

	rw_lock child_lock;
	array<blockhdl> children;
	blockhdl *parent;

	inline int index(int i, int j, int k, int width = cubes_per_side+1);
	void generate(void *data = NULL);
	void generate(array<grad3f> svalues, vec3i offset, void *data = NULL);
	void split(void *data = NULL);
	void merge();
	void load();
	void unload();
	void render(int vertex_location, int origin_location);
	bool contains(vec3F location, float radius);
};

#endif
