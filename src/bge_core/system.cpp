#include "system.h"

systemhdl::systemhdl()
{
	seed = rand();

	sun = NULL;
	num_planets = 0;
}

systemhdl::systemhdl(unsigned int s)
{
	seed = s;

	sun = NULL;
	num_planets = 0;
}

systemhdl::~systemhdl()
{
	seed = 0;

	sun = NULL;
	num_planets = 0;
}

void systemhdl::initialize_derived()
{
	type = OBJ_SYSTEM;

	srand(seed);

	num_planets = rand() % (1 + max_planets - min_planets) + min_planets;

	sun = new starhdl(seed);
	sun->initialize();
	add_child(sun);

	planethdl *planet;
	double r = 0;
	double t = 0;
	for (int x = 0; x < num_planets; x++)
	{
		planet = new planethdl(seed + 1 + x);
		planet->initialize();

		r += double(rand()%1000)*50000;
		t += double(rand()%7200)*m_pi/3600.0;
		planet->position = location(r*cos(t), 0.0, r*sin(t));
		add_child(planet);
	}

	radius = r + 500000;
}

void systemhdl::release_derived()
{
	seed = 0;

	num_planets = 0;
	sun = NULL;
}

void systemhdl::prepare_derived()
{
	objecthdl* curr = children_s;
	while (curr != NULL)
	{
		if (curr != NULL && !curr->remove)
			curr->prepare();
		if (curr != NULL)
			curr = curr->next;
	}
}

void systemhdl::render_derived()
{
}

void systemhdl::generate_info(containerhdl *contain)
{
}

void systemhdl::generate_menu(containerhdl *contain)
{
}
