#include "value.h"
#include "../math.h"

void valuehdl::init(char *n, void *v, value_t t)
{
	strcpy(name, n);
	value = v;
	type = t;
}

void valuehdl::render(fonthdl *font)
{
	if (value != NULL)
	{
		char out[256];
		if (type == bool_ty)
			sprintf(out, "%d", (int)(*((bool*)value)));
		if (type == int_ty)
			sprintf(out, "%d", *(int*)value);
		if (type == float_ty)
			sprintf(out, "%f", *(float*)value);
		if (type == double_ty)
			sprintf(out, "%f", (float)*(double*)value);
		if (type == vec_ty)
			sprintf(out, "%f %f %f", (float)((vec*)value)->x, (float)((vec*)value)->y, (float)((vec*)value)->z);
		if (type == location_ty)
		{
			vec output = ((location*)value)->tovec();
			sprintf(out, "%f %f %f", (float)output.x, (float)output.y, (float)output.z);
		}

		font->print(name, rgba_hdr(1.0, 1.0, 0.0, 1.0), vec2f(0.0, -0.0275));
		if (type != texture_ty)
			font->print(out, rgba_hdr(0.0, 1.0, 0.0, 1.0), vec2f(0.0, -0.0625));
	}
}

void valuehdl::release()
{
	value = NULL;
}
