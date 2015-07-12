#include "vector.h"
#include "geometry.h"
#include "gradient.h"

#ifndef noise_h
#define noise_h

namespace core
{

#define noise_size 64
#define noise_mask 63

struct noise1hdl
{
	noise1hdl();
	noise1hdl(int s);
	~noise1hdl();

	int seed;
	float data[noise_size*2]; // permutations and gradients

	void initialize(int s);

	grad1f operator()(vec1f n, float r = 0.0, float m = 0.0) const;
};

struct noise2hdl
{
	noise2hdl();
	noise2hdl(int s);
	~noise2hdl();

	int seed;
	float data[noise_size*3]; // permutations and gradients

	void initialize(int s);

	grad2f operator()(vec2f n, float r = 0.0, float m = 0.0) const;
};

struct noise3hdl
{
	noise3hdl();
	noise3hdl(int s);
	~noise3hdl();

	int seed;
	float data[noise_size*4]; // permutations and gradients

	void initialize(int s);

	grad3f operator()(vec3f n, float r = 0.0, float m = 0.0) const;
};

/*
 * Procedural fBm evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  is the fractal increment parameter
 *    ``lacunarity''  is the gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 */
double fBm(vec3f point, double H, double lacunarity, double octaves, const noise3hdl &noise = noise3hdl(0));

/*
 * Procedural multifractal evaluated at "point";
 * returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  determines the highest fractal dimension
 *    ``lacunarity''  is gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 *    ``offset''  is the zero offset, which determines multifractality
 */
double multifractal(vec3f point, double H, double lacunarity, double octaves, double offset, const noise3hdl &noise = noise3hdl(0));

/* Ridged multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           1.0
 *      offset:      1.0
 *      gain:        2.0
 */
double ridge_noise(vec3f point, double H, double lacunarity, double octaves, double offset, double gain, const noise3hdl &noise = noise3hdl(0));

}

#endif
