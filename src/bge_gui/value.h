#include "../common.h"
#include "font.h"

#ifndef value_h
#define value_h

enum value_t
{
	bool_ty,
	int_ty,
	float_ty,
	double_ty,
	texture_ty,
	vec_ty,
	location_ty
};

struct valuehdl
{
	char name[256];
	void *value;
	value_t type;

	void init(char *n, void *v, value_t t);
	void render(fonthdl *font);
	void release();
};

#endif
