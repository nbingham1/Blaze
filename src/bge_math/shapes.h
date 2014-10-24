#include "basic.h"
#include "vector.h"

#ifndef shapes_h
#define shapes_h

struct planehdl
{
	planehdl();
	planehdl(vec v1, vec v2, vec v3);
	~planehdl();

	vec normal;
	double offset;

	void generate(vec v1, vec v2, vec v3);
	double distance(vec v);
};

struct frustumhdl
{
	frustumhdl();
	~frustumhdl();

	float leftf, rightf;
	float downf, upf;
	float nearf, farf;

	vec base[6];
	vec transform[6];

	void initialize(double l, double r, double d, double u, double n, double f);
	void set_orientation(vec angle);
	void rotate(vec angle);

	double dist(vec point);
};

struct space
{
	space();
	~space();

	vec x, y, z;

	void rotate_xyz(vec angle);
	void rotate_zyx(vec angle);
	void orient_xyz(vec angle);
	void orient_zyx(vec angle);

	vec transform(vec vector);
};

struct sphere
{
	sphere();
	sphere(vec c, double r);
	~sphere();

	vec center;
	double radius;

	bool intersect_ray(vec r, vec *p, vec *n);
};

int intersect_ray_triangle(vec orig, vec dir, vec vert0, vec vert1, vec vert2, double *t, double *u, double *v);

#endif
