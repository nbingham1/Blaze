#ifndef mathdef_h
#define mathdef_h

#include <math.h>
#include "vector.h"

#define m_pi 3.14159265358979323
#define m_e 2.718281828
#define m_epsilon 0.000001

#define m_sqr(n) (n*n)
#define m_cube(n) (n*n*n)

#define m_abs(n) ((n) < 0.0 ? -(n) : (n))
#define m_min(m, n) (m < n ? m : n)
#define m_max(m, n) (m < n ? n : m)

#define m_nan(n) (!(n > 0 || n <= 0))

#define m_clamp(n, a, b) m_min(m_max(n, a), b)

#define degtorad(a) m_pi*a/180.0
#define radtodeg(a) 180.0*a/m_pi

void noiseinit(void);
double noise1(double);
double noise2(double *);
double noise3(double *);
void normalize3(double *);
void normalize2(double *);

double PerlinNoise1D(double,double,double,int);
double PerlinNoise2D(double,double,double,double,int);
double PerlinNoise3D(double,double,double,double,double,int);
double Brownian(double x, double y, double z, double alpha, double *exponent, double octaves);

void noise3tex(unsigned char *tex, int size);

int intraytri(vec orig, vec dir, vec vert0, vec vert1, vec vert2, double *t, double *u, double *v);

#endif
