#include "../common.h"
#include "object.h"
#include "star.h"
#include "planet.h"

#ifndef system_h
#define system_h

#define OBJ_SYSTEM 4

const int min_stars = 1;
const int max_stars = 2;
const int min_planets = 0;
const int max_planets = 15;

struct systemhdl : objecthdl
{
private:
	virtual void initialize_derived();
	virtual void release_derived();

	virtual void prepare_derived();
	virtual void render_derived();

public:
	systemhdl();
	systemhdl(unsigned int s);
	~systemhdl();

	unsigned int seed;

	int num_planets;

	starhdl *sun;

	virtual void generate_info(containerhdl *contain);
	virtual void generate_menu(containerhdl *contain);
};

#endif
