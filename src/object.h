#include "vector.h"
#include "mathematics.h"
#include "standard.h"
#include "graphics.h"
#include "physics.h"
//#include "material.h"
#include "camera.h"
#include "shader.h"

#ifndef object_h
#define object_h

struct objecthdl
{
	physicshdl physics;
//	materialhdl material;

	objecthdl *next, *prev;

	void init();
	void render();
};

#endif
