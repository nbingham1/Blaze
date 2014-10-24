#include "mathdef.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i,b0,b1,r0,r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

static int p[B + B + 2];
static double g3[B + B + 2][3];
static double g2[B + B + 2][2];
static double g1[B + B + 2];
static int start = 1;

double noise1(double arg)
{
   int bx0, bx1;
   double rx0, rx1, sx, t, u, v, vec[1];

   vec[0] = arg;
   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0,bx0,bx1,rx0,rx1);

   sx = s_curve(rx0);
   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return(lerp(sx, u, v));
}

double noise2(double vec[2])
{
   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);
   sy = s_curve(ry0);

   q = g2[ b00 ] ; u = at2(rx0,ry0);
   q = g2[ b10 ] ; v = at2(rx1,ry0);
   a = lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1);
   q = g2[ b11 ] ; v = at2(rx1,ry1);
   b = lerp(sx, u, v);

   return lerp(sy, a, b);
}

double noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);
   setup(2, bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = lerp(t, u, v);

   c = lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = lerp(t, u, v);

   d = lerp(sy, a, b);

   return lerp(sz, c, d);
}

void normalize2(double v[2])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
}

void normalize3(double v[3])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
   v[2] = v[2] / s;
}

void noiseinit(void)
{
   int i, j, k;

   for (i = 0 ; i < B ; i++) {
      p[i] = i;
      g1[i] = (double)((rand() % (B + B)) - B) / B;

      for (j = 0 ; j < 2 ; j++)
         g2[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize2(g2[i]);

      for (j = 0 ; j < 3 ; j++)
         g3[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = rand() % B];
      p[j] = k;
   }

   for (i = 0 ; i < B + 2 ; i++) {
      p[B + i] = p[i];
      g1[B + i] = g1[i];
      for (j = 0 ; j < 2 ; j++)
         g2[B + i][j] = g2[i][j];
      for (j = 0 ; j < 3 ; j++)
         g3[B + i][j] = g3[i][j];
   }
}

double PerlinNoise1D(double x,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p,scale = 1;

   p = x;
   for (i=0;i<n;i++) {
      val = noise1(p);
      sum += val / scale;
      scale *= alpha;
      p *= beta;
   }
   return(sum);
}

double PerlinNoise2D(double x,double y,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p[2],scale = 1;

   p[0] = x;
   p[1] = y;
   for (i=0;i<n;i++) {
      val = noise2(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
   }
   return(sum);
}

double PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p[3],scale = 1;

   p[0] = x;
   p[1] = y;
   p[2] = z;
   for (i=0;i<n;i++) {
      val = noise3(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
      p[2] *= beta;
   }
	return(sum);
}

double Brownian(double x, double y, double z, double alpha, double *exponent, double octaves)
{
	double p[3];
	double oct = octaves;

	p[0] = x;
	p[1] = y;
	p[2] = z;

	float fValue = 0.0f;
	int i;
	for(i = 0; i < (int)oct; i++)
	{
		fValue += noise3(p) * exponent[i];
		for(int j = 0; j < 3; j++)
			p[j] *= alpha;
	}

	oct -= (int)oct;
	fValue += oct * noise3(p) * exponent[i];

	if(fValue <= 0.0)
		return -pow(-fValue, 0.7);

	return pow(fValue, 1.0 + noise3(p) * fValue);
}

void noise3tex(unsigned char *tex, int size)
{
	int f, i, j, k, inc;
	int startf = 4;
	int numo = 4;
	double ni[3];
	double inci, incj, inck;
	int freq = startf;
	unsigned char *ptr;
	double amp = 0.5;

	for (f = 0, inc = 0; f < numo; ++f, freq *= 2, ++inc, amp *= 0.5)
	{
		ptr = tex;
		ni[0] = ni[1] = ni[2] = 0;

		inci = 1.0/(size/freq);
		for (i = 0; i < size; ++i, ni[0] += inci)
		{
			incj = 1.0/(size/freq);
			for (j = 0; j < size; ++j, ni[1] += incj)
			{
				inck = 1.0/(size/freq);
				for (k = 0; k < size; ++k, ni[2] += inck, ptr += 4)
					*(ptr+inc) = (unsigned char)((noise3(ni) + 1.0)*amp*128.0);
			}
		}
	}
}

int intraytri(vec orig, vec dir, vec vert0, vec vert1, vec vert2, double *t, double *u, double *v)
{
	bool cull = true;
	vec edge1, edge2, tvec, pvec, qvec;
	double det , inv_det;
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	pvec = cross(dir, edge2);
	det = dot(edge1, pvec);

	if (cull)
	{
		if (det < m_epsilon)
			return 0;
		tvec = orig - vert0;
		*u = dot(tvec, pvec);
		if (*u < 0.0 || *u > det)
			return 0;
		qvec = cross(tvec, edge1);
		*v = dot(dir, qvec);
		if (*v < 0.0 || *u + *v > det)
			return 0;
		*t = dot(edge2, qvec);
		inv_det = 1.0 / det;
		*t *= inv_det;
		*u *= inv_det;
		*v *= inv_det;
	}
	else
	{
		if (det > -m_epsilon && det < m_epsilon)
			return 0;
		inv_det = 1.0 / det;
		tvec = orig - vert0;
		*u = dot(tvec, pvec) * inv_det;
		if (*u < -m_epsilon || *u > 1.0 + m_epsilon)
			return 0;
		qvec = cross(tvec, edge1);
		*v = dot(dir, qvec) * inv_det;
		if (*v < -m_epsilon || *u + *v > 1.0 + m_epsilon)
			return 0;
		*t = dot(edge2, qvec) * inv_det;
	}
	return 1;
}

