#include "basic.h"

#ifndef vertex_h
#define vertex_h

// vertex structure used for geometry data in VBO
// Interleaved buffer for performance
struct vertex_v3n3t3g3
{
	// sphere definition
	float v[3];
	float n[3];
	float t[3];

	// terrain offset
	float g[2];
	float dn[3];

	vertex_v3n3t3g3 &operator=(vertex_v3n3t3g3 v);
	void copywithshift(vertex_v3n3t3g3 v, float *voff);
} __attribute__ ((packed));

struct vertex_v3t1
{
	float v[3];
	float t;
} __attribute__ ((packed));

#endif
