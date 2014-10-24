#include "location.h"
#include "../common.h"

location1D::location1D()
{

}

location1D::location1D(vec v) : vec(v)
{

}

location1D::~location1D()
{

}

void location1D::eq()
{
	if (z > 0.1 && y < -0.1)
	{
		z -= 1.0;
		y += 1e6;
	}
	if (z < -0.1 && y > 0.1)
	{
		z += 1.0;
		y -= 1e6;
	}

	if (y > 0.1 && x < -0.1)
	{
		y -= 1.0;
		x += 1e6;
	}
	if (y < -0.1 && x > 0.1)
	{
		y += 1.0;
		x -= 1e6;
	}

	double v, s;

	v = x/1e6;
	s = v < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	y += v;
	x -= v*1e6;

	v = y/1e6;
	s = v < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	z += v;
	y -= v*1e6;

	v = z;
	s = v < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	y = y + (z - v)*1e6;
	z = v;

	v = y;
	s = v < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	x = x + (y - v)*1e6;
	y = v;
}

double location1D::todouble()
{
	return x + y*1e6 + z*1e12;
}

bool operator<(location1D l1, location1D l2)
{
	return (l1.z < l2.z) || (l1.z == l2.z && l1.y < l2.y) || (l1.z == l2.z && l1.y == l2.y && l1.x < l2.x);
}

bool operator<(location1D l, double d)
{
	return l.todouble() < d;
}

bool operator>(location1D l1, location1D l2)
{
	return (l1.z > l2.z) || (l1.z == l2.z && l1.y > l2.y) || (l1.z == l2.z && l1.y == l2.y && l1.x > l2.x);
}

bool operator>(location1D l, double d)
{
	return l.todouble() > d;
}

location::location()
{

}

location::location(double x, double y, double z)
{
	meters.x = x - floor(x/1e6)*1e6;
	megameters.x = floor(x/1e6) - floor(x/1e12)*1e6;
	terameters.x = floor(x/1e12);

	meters.y = y - floor(y/1e6)*1e6;
	megameters.y = floor(y/1e6) - floor(y/1e12)*1e6;
	terameters.y = floor(y/1e12);

	meters.z = z - floor(z/1e6)*1e6;
	megameters.z = floor(z/1e6) - floor(z/1e12)*1e6;
	terameters.z = floor(z/1e12);
}

location::~location()
{

}

void location::eq()
{
	if (terameters.x > 0.1 && megameters.x < -0.1)
	{
		terameters.x -= 1.0;
		megameters.x += 1e6;
	}
	if (terameters.x < -0.1 && megameters.x > 0.1)
	{
		terameters.x += 1.0;
		megameters.x -= 1e6;
	}

	if (megameters.x > 0.1 && meters.x < -0.1)
	{
		megameters.x -= 1.0;
		meters.x += 1e6;
	}
	if (megameters.x < -0.1 && meters.x > 0.1)
	{
		megameters.x += 1.0;
		meters.x -= 1e6;
	}


	if (terameters.y > 0.1 && megameters.y < -0.1)
	{
		terameters.y -= 1.0;
		megameters.y += 1e6;
	}
	if (terameters.y < -0.1 && megameters.y > 0.1)
	{
		terameters.y += 1.0;
		megameters.y -= 1e6;
	}

	if (megameters.y > 0.1 && meters.y < -0.1)
	{
		megameters.y -= 1.0;
		meters.y += 1e6;
	}
	if (megameters.y < -0.1 && meters.y > 0.1)
	{
		megameters.y += 1.0;
		meters.y -= 1e6;
	}

	if (terameters.z > 0.1 && megameters.z < -0.1)
	{
		terameters.z -= 1.0;
		megameters.z += 1e6;
	}
	if (terameters.z < -0.1 && megameters.z > 0.1)
	{
		terameters.z += 1.0;
		megameters.z -= 1e6;
	}

	if (megameters.z > 0.1 && meters.z < -0.1)
	{
		megameters.z -= 1.0;
		meters.z += 1e6;
	}
	if (megameters.z < -0.1 && meters.z > 0.1)
	{
		megameters.z += 1.0;
		meters.z -= 1e6;
	}

	vec v, s;

	v = meters/1e6;
	s.x = v.x < 0.0 ? -1.0 : 1.0;
	s.y = v.y < 0.0 ? -1.0 : 1.0;
	s.z = v.z < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	megameters += v;
	meters -= v*1e6;

	v = megameters/1e6;
	s.x = v.x < 0.0 ? -1.0 : 1.0;
	s.y = v.y < 0.0 ? -1.0 : 1.0;
	s.z = v.z < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	terameters += v;
	megameters -= v*1e6;

	v = terameters;
	s.x = v.x < 0.0 ? -1.0 : 1.0;
	s.y = v.y < 0.0 ? -1.0 : 1.0;
	s.z = v.z < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	megameters = megameters + (terameters - v)*1e6;
	terameters = v;

	v = megameters;
	s.x = v.x < 0.0 ? -1.0 : 1.0;
	s.y = v.y < 0.0 ? -1.0 : 1.0;
	s.z = v.z < 0.0 ? -1.0 : 1.0;
	v = floor(abs(v))*s;
	meters = meters + (megameters - v)*1e6;
	megameters = v;
}

vec location::tovec()
{
	return meters + megameters*1e6 + terameters*1e12;
}

location &location::operator=(location l)
{
	this->meters = l.meters;
	this->megameters = l.megameters;
	this->terameters = l.terameters;
	return *this;
}

location &location::operator+=(location l)
{
	*this = *this + l;
	return *this;
}

location &location::operator-=(location l)
{
	*this = *this - l;
	return *this;
}

location &location::operator=(vec v)
{
	meters.x = v.x - floor(v.x/1e6)*1e6;
	megameters.x = floor(v.x/1e6) - floor(v.x/1e12)*1e6;
	terameters.x = floor(v.x/1e12);

	meters.y = v.y - floor(v.y/1e6)*1e6;
	megameters.y = floor(v.y/1e6) - floor(v.y/1e12)*1e6;
	terameters.y = floor(v.y/1e12);

	meters.z = v.z - floor(v.z/1e6)*1e6;
	megameters.z = floor(v.z/1e6) - floor(v.z/1e12)*1e6;
	terameters.z = floor(v.z/1e12);

	return *this;
}

location &location::operator+=(vec v)
{
	*this = *this + v;
	return *this;
}

location &location::operator-=(vec v)
{
	*this = *this - v;
	return *this;
}

location &location::operator*=(vec v)
{
	*this = *this * v;
	return *this;
}

location &location::operator/=(vec v)
{
	*this = *this / v;
	return *this;
}

location operator-(location l)
{
	return l*-1.0;
}

location operator+(location l1, location l2)
{
	location temp;
	temp.meters = l1.meters + l2.meters;
	temp.megameters = l1.megameters + l2.megameters;
	temp.terameters = l1.terameters + l2.terameters;

	temp.eq();

	return temp;
}

location operator-(location l1, location l2)
{
	location temp;
	temp.meters = l1.meters - l2.meters;
	temp.megameters = l1.megameters - l2.megameters;
	temp.terameters = l1.terameters - l2.terameters;

	temp.eq();

	return temp;
}

location operator+(location l, vec v)
{
	location temp = l;
	temp.meters += v;

	temp.eq();

	return temp;
}

location operator-(location l, vec v)
{
	location temp = l;
	temp.meters -= v;

	temp.eq();

	return temp;
}

location operator*(location l, vec v)
{
	location temp = l;
	temp.meters *= v;
	temp.megameters *= v;
	temp.terameters *= v;

	temp.eq();

	return temp;
}

location operator/(location l, vec v)
{
	location temp = l;
	temp.meters /= v;
	temp.megameters /= v;
	temp.terameters /= v;

	temp.eq();

	return temp;
}

location operator*(location l, double f)
{
	location temp = l;
	temp.meters *= f;
	temp.megameters *= f;
	temp.terameters *= f;

	temp.eq();

	return temp;
}

location operator/(location l, double f)
{
	location temp = l;
	temp.meters /= f;
	temp.megameters /= f;
	temp.terameters /= f;

	temp.eq();

	return temp;
}

location cross(location v1, location v2)
{
	location result;
	result.meters = cross(v1.meters, v2.meters);
	result.megameters = cross(v1.megameters, v2.megameters);
	result.terameters = cross(v1.terameters, v2.terameters);
	result.eq();
	return result;
}

location1D dot(location v1, location v2)
{
	location1D result;
	result.x = dot(v1.meters, v2.meters);
	result.y = dot(v1.megameters, v2.megameters);
	result.z = dot(v1.terameters, v2.terameters);
	result.eq();
	return result;
}
