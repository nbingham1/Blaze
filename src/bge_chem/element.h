#ifndef element_h
#define element_h

struct elementhdl
{
	elementhdl();
	~elementhdl();

	char symbol[2];

	short protons;

	double atomic_mass;

	double *spectra;
	int num_spectra;
	short *ion_index;
	char num_ions;

	bool load(const char *filename);
	void release();
};

#endif
