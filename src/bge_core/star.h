#include "../common.h"
#include "object.h"

#ifndef star_h
#define star_h

#define OBJ_STAR 3

struct starhdl : objecthdl
{
private:
	virtual void initialize_derived();
	virtual void release_derived();

	virtual void prepare_derived();
	virtual void render_derived();

public:
	starhdl();
	starhdl(unsigned int s);
	~starhdl();

	unsigned int seed;

	virtual void generate_menu(containerhdl *contain);
};

#endif
