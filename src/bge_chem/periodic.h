#include "element.h"
#include "../bge_mtrl.h"

#ifndef periodic_h
#define periodic_h

struct periodichdl
{
	periodichdl();
	~periodichdl();

	elementhdl *table;
	short lookup[6682];
	short table_size;

	double *wbgr;
	short wbgr_size;

	bool load(const char *filename, int n);
	void release();

	rgb_hdr get_color(double wavelength, double min, double max);
	int get_color_scheme(char *compounds, rgb_hdr *scheme, float *percents);

	void generate_texture(char *compounds, double grain_size, double temperature, int size, unsigned char *col, unsigned char *normal, unsigned char *spec);
};

#endif
