/*
 * particle.h
 *
 *  Created on: Mar 31, 2014
 *      Author: nbingham
 */

#ifndef particle_h
#define particle_h

struct particlehdl
{
	double mass;
	vec3F position;
	vec3F velocity;
	vec3F acceleration;
	vec3F momentum;
	vec3f forces;
};

#endif
