#include "complex.h"
#include "../common.h"

complex::complex()
{
	this->a = 0.0;
	this->b = 0.0;
}

complex::complex(float a, float b) // cartesian, value 1, value 2
{
	this->a = a;
	this->b = b;
}

complex::~complex()
{
}

complex &complex::operator=(complex a)
{
	this->a = a.a;
	this->b = a.b;
	return *this;
}

complex &complex::operator+=(complex a)
{
	*this = *this + a;
	return *this;
}

complex &complex::operator-=(complex a)
{
	*this = *this - a;
	return *this;
}

complex &complex::operator*=(complex a)
{
	*this = *this * a;
	return *this;
}

complex &complex::operator/=(complex a)
{
	*this = *this / a;
	return *this;
}

complex &complex::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

complex &complex::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

complex &complex::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

complex &complex::operator/=(float a)
{
	*this = *this / a;
	return *this;
}


complex operator+(complex a, complex b)
{
	return complex(a.a + b.a, a.b + b.b);
}

complex operator-(complex a, complex b)
{
	return complex(a.a - b.a, a.b - b.b);
}

complex operator*(complex a, complex b)
{
	return complex(a.a*b.a - a.b*b.b, a.b*b.a + a.a*b.b);
}

complex operator/(complex a, complex b)
{
	return complex((a.a*b.a - a.b*b.b)/(b.a*b.a + b.b*b.b), (a.b*b.a - a.a*b.b)/(b.a*b.a + b.b*b.b));
}

complex operator+(complex a, float b)
{
	return complex(a.a + b, a.b + b);
}

complex operator-(complex a, float b)
{
	return complex(a.a - b, a.b - b);
}

complex operator*(complex a, float b)
{
	return complex(a.a * b, a.b * b);
}

complex operator/(complex a, float b)
{
	return complex(a.a / b, a.b / b);
}

complex operator+(float a, complex b)
{
	return complex(a + b.a, a + b.a);
}

complex operator-(float a, complex b)
{
	return complex(a - b.a, a - b.b);
}

complex operator*(float a, complex b)
{
	return complex(a * b.a, a * b.b);
}

complex operator/(float a, complex b)
{
	return complex(a / b.a, a / b.b);
}

complex conjugate(complex a)
{
	return complex(a.a, -a.b);
}

float magnitude(complex a)
{
	return sqrt(a.a*a.a + a.b*a.b);
}
