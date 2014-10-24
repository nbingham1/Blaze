#include "newtonian.h"

double gravitational_force(objecthdl *o1, objecthdl *o2)
{
	return o1->mass*gamma(o1)*o2->mass*gamma(o2)*6.673e-11/mag2((o1->position - o2->position).tovec());
}

double gamma(objecthdl *o)
{
	return 1.0/sqrt(1.0-(mag2(o->velocity.tovec())/(299792458.0*299792458.0)));
}
