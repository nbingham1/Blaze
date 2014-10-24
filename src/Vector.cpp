#include "vector.h"
#include "mathematics.h"
#include "graphics.h"
#include "standard.h"

vec::vec()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 0.0;
}

vec::vec(double x)
{
	this->x = x;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 0.0;
}

vec::vec(double x, double y)
{
	this->x = x;
	this->y = y;
	this->z = 0.0;
	this->w = 0.0;
}

vec::vec(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 0.0;
}

vec::vec(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec::vec(double *xyzw, int num)
{
	this->x = num > 0 ? xyzw[0] : 0.0;
	this->y = num > 1 ? xyzw[1] : 0.0;
	this->z = num > 2 ? xyzw[2] : 0.0;
	this->w = num > 3 ? xyzw[3] : 0.0;
}

vec::vec(float *xyzw, int num)
{
	this->x = num > 0 ? xyzw[0] : 0.0;
	this->y = num > 1 ? xyzw[1] : 0.0;
	this->z = num > 2 ? xyzw[2] : 0.0;
	this->w = num > 3 ? xyzw[3] : 0.0;
}

vec &vec::operator=(vec v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
	return *this;
}

vec &vec::operator+=(vec v)
{
	*this = *this + v;
	return *this;
}

vec &vec::operator-=(vec v)
{
	*this = *this - v;
	return *this;
}

vec &vec::operator*=(vec v)
{
	*this = *this * v;
	return *this;
}

vec &vec::operator/=(vec v)
{
	*this = *this / v;
	return *this;
}

vec &vec::operator+=(double f)
{
	*this = *this + f;
	return *this;
}

vec &vec::operator-=(double f)
{
	*this = *this - f;
	return *this;
}

vec &vec::operator*=(double f)
{
	*this = *this * f;
	return *this;
}

vec &vec::operator/=(double f)
{
	*this = *this / f;
	return *this;
}

float vec::operator[](int index)
{
	switch (index)
	{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		case 3:
			return w;
			break;
		default:
			printf("Error: Out of bounds");
			return 0;
			break;
	}
}

vec operator-(vec v)
{
	return vec(-v.x, -v.y, -v.z, -v.w);
}

vec operator+(vec v1, vec v2)
{
	return vec(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec operator-(vec v1, vec v2)
{
	return vec(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec operator*(vec v1, vec v2)
{
	return vec(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec operator/(vec v1, vec v2)
{
	return vec(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

vec operator+(double f, vec v)
{
	return vec(f + v.x, f + v.y, f + v.z, f + v.w);
}

vec operator-(double f, vec v)
{
	return vec(f - v.x, f - v.y, f - v.z, f - v.w);
}

vec operator*(double f, vec v)
{
	return vec(f * v.x, f * v.y, f * v.z, f * v.w);
}

vec operator/(double f, vec v)
{
	return vec(f / v.x, f / v.y, f / v.z, f / v.w);
}

vec operator+(vec v, double f)
{
	return vec(v.x + f, v.y + f, v.z + f, v.w + f);
}

vec operator-(vec v, double f)
{
	return vec(v.x - f, v.y - f, v.z - f, v.w - f);
}

vec operator*(vec v, double f)
{
	return vec(v.x * f, v.y * f, v.z * f, v.w * f);
}

vec operator/(vec v, double f)
{
	return vec(v.x / f, v.y / f, v.z / f, v.w / f);
}

bool operator==(vec v1, vec v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w)
		return true;

	return false;
}

vec abs(vec v)
{
	return vec(m_abs(v.x), m_abs(v.y), m_abs(v.z), m_abs(v.w));
}

vec norm(vec v)
{
	return v/mag(v);
}

vec cross(vec v1, vec v2)
{
	return vec(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

vec cross(vec v1, vec v2, vec v3)
{
	return vec( v1.y*(v2.z*v3.w - v3.z*v2.w) - v1.z*(v2.y*v3.w - v3.y*v2.w) + v1.w*(v2.y*v3.z - v3.y*v2.z),
			   -v1.x*(v2.z*v3.w - v3.z*v2.w) + v1.z*(v2.x*v3.w - v3.x*v2.w) - v1.w*(v2.x*v3.z - v3.x*v2.z),
			    v1.x*(v2.y*v3.w - v3.y*v2.w) - v1.y*(v2.x*v3.w - v3.x*v2.w) + v1.w*(v2.x*v3.y - v3.x*v2.y),
			   -v1.x*(v2.y*v3.z - v3.y*v2.z) + v1.y*(v2.x*v3.z - v3.x*v2.z) - v1.z*(v2.x*v3.y - v3.x*v2.y));
}

vec rotx(vec v, double a)
{
	return vec(v.x, v.y*cos(a) - v.z*sin(a), v.y*sin(a) + v.z*cos(a));
}

vec roty(vec v, double a)
{
	return vec(v.x*cos(a) + v.z*sin(a), v.y, -v.x*sin(a) + v.z*cos(a));
}

vec rotz(vec v, double a)
{
	return vec(v.x*cos(a) - v.y*sin(a), v.x*sin(a) + v.y*cos(a), v.z);
}

double mag(vec v)
{
	return sqrt(m_sqr(v.x) + m_sqr(v.y) + m_sqr(v.z) + m_sqr(v.w));
}

double dot(vec v1, vec v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w);
}

double dist(vec v1, vec v2)
{
	return mag(v2 - v1);
}
