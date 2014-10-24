#include "basic.h"
#include "vector.h"
#include "../common.h"

#ifndef noise_h
#define noise_h

#define noise_size 64
#define noise_mask 63

struct noisevec
{
	noisevec();
	noisevec(float height);
	~noisevec();

	float dx, dy, dz;
	float h;

	noisevec &operator=(noisevec v);
	vec normal(vec r, vec t);
};

noisevec operator-(noisevec v);

noisevec operator+(noisevec v1, noisevec v2);
noisevec operator-(noisevec v1, noisevec v2);
noisevec operator*(noisevec v1, noisevec v2);
noisevec operator/(noisevec v1, noisevec v2);

noisevec operator+(noisevec v1, float v2);
noisevec operator-(noisevec v1, float v2);
noisevec operator*(noisevec v1, float v2);
noisevec operator/(noisevec v1, float v2);

noisevec operator+(float v1, noisevec v2);
noisevec operator-(float v1, noisevec v2);
noisevec operator*(float v1, noisevec v2);
noisevec operator/(float v1, noisevec v2);

struct noise1hdl
{
	noise1hdl();
	~noise1hdl();

	int seed;
	float data[noise_size*2]; // permutations and gradients

	void initialize(int s);

	noisevec operator()(vec n, float r, float m);
};

struct noise2hdl
{
	noise2hdl();
	~noise2hdl();

	int seed;
	float data[noise_size*3]; // permutations and gradients

	void initialize(int s);

	noisevec operator()(vec n, float r, float m);
};

struct noise3hdl
{
	noise3hdl();
	~noise3hdl();

	// basic noise
	int seed;
	float data[noise_size*4]; // permutations and gradients

	void initialize(int s);

	noisevec operator()(vec n, float r, float m);
};



#endif
