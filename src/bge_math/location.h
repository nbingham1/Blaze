#include "basic.h"
#include "vector.h"

#ifndef location_h
#define location_h

struct location1D : vec
{
	location1D();
	location1D(vec v);
	~location1D();

	void eq();
	double todouble();
};

bool operator<(location1D l1, location1D l2);
bool operator<(location1D l, double d);
bool operator>(location1D l1, location1D l2);
bool operator>(location1D l, double d);

struct location
{
	location();
	location(double x, double y, double z);
	~location();

	vec meters;
	vec megameters;
	vec terameters;

	void eq();
	vec tovec();

	location &operator=(location l);
	location &operator+=(location l);
	location &operator-=(location l);

	location &operator=(vec v);
	location &operator+=(vec v);
	location &operator-=(vec v);
	location &operator*=(vec v);
	location &operator/=(vec v);
};

location operator-(location l);

location operator+(location l1, location l2);
location operator-(location l1, location l2);

location operator+(location l, vec v);
location operator-(location l, vec v);
location operator*(location l, vec v);
location operator/(location l, vec v);

location operator*(location l, double f);
location operator/(location l, double f);

location cross(location v1, location v2);
location1D dot(location v1, location v2);

#endif
