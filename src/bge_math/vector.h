#include "basic.h"

#ifndef vector_h
#define vector_h

struct vec
{
	vec();
	vec(double x);
	vec(double x, double y);
	vec(double x, double y, double z);
	vec(double x, double y, double z, double w);
	vec(double *xyzw, int num);
	vec(float *xyzw, int num);

	double x, y, z, w;

	vec &operator=(vec v);
	vec &operator+=(vec v);
	vec &operator-=(vec v);
	vec &operator*=(vec v);
	vec &operator/=(vec v);

	vec &operator+=(double f);
	vec &operator-=(double f);
	vec &operator*=(double f);
	vec &operator/=(double f);

	double operator[](int index);
};

vec operator-(vec v);

vec operator+(vec v1, vec v2);
vec operator-(vec v1, vec v2);
vec operator*(vec v1, vec v2);
vec operator/(vec v1, vec v2);

vec operator+(double f, vec v);
vec operator-(double f, vec v);
vec operator*(double f, vec v);
vec operator/(double f, vec v);

vec operator+(vec v, double f);
vec operator-(vec v, double f);
vec operator*(vec v, double f);
vec operator/(vec v, double f);

bool operator==(vec v1, vec v2);

vec abs(vec v);
vec norm(vec v);
float magnorm(vec *v);
vec cross(vec v1, vec v2);
vec cross(vec v1, vec v2, vec v3);
vec rotx(vec v, double a);
vec roty(vec v, double a);
vec rotz(vec v, double a);
vec slerp(vec v1, vec v2, double t);
vec floor(vec v);

vec cartesiantospherical(vec v);
vec sphericaltocartesian(vec v);
vec cartesiantocylindrical(vec v);
vec cylindricaltocartesian(vec v);

double mag(vec v);
double mag2(vec v);
double dot(vec v1, vec v2);
double dist(vec v1, vec v2);

#endif
