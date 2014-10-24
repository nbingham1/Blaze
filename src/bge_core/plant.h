/*
 * plant.h
 *
 *  Created on: Jan 26, 2014
 *      Author: nbingham
 */

#include "object.h"
#include "../common.h"

#ifndef plant_h
#define plant_h

#define OBJ_PLANT 5

struct planthdl;
struct branchhdl;

struct plantvtx
{
	float v[3];
	float n[3];
	float t[2];
};

struct plantprt_type
{
	plantprt_type();
	~plantprt_type();

	char symbol;
	plantvtx *geometry;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vx;
	GLuint ft;
	GLuint pm;
	texture2hdl tx;
	vec delta_pos;
	vec delta_ang;
	bool push_delta;
	bool pop_delta;

	int num_vertices;
	int num_indices;

	void render(list<pair<vec, space> > *offset, float age);
};

struct plantprt
{
	plantprt();
	plantprt(float age, plantprt_type *type);
	~plantprt();

	float age;
	plantprt_type *type;
};

struct plantpattern : list<plantprt>
{
	plantpattern();
	plantpattern(const plantpattern &pattern);
	plantpattern(string p, list<plantprt_type> *types);
	~plantpattern();
};

struct plantrule
{
	plantrule();
	plantrule(string f, string t, list<plantprt_type> *types);
	~plantrule();

	plantpattern from;
	plantpattern to;
};

struct planthdl : objecthdl
{
private:
	virtual void initialize_derived();
	virtual void release_derived();
	virtual void prepare_derived();
	virtual void render_derived();

public:
	planthdl();
	~planthdl();

	plantpattern parts;
	list<plantprt_type> types;
	list<plantrule> rules;

	double delta;
	double length;
};

#endif
