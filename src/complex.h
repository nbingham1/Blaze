#ifndef complex_h
#define complex_h

struct complex
{
	complex();
	complex(float a, float b); // cartesian, value 1, value 2
	~complex();

	float a;
	float b;

	complex &operator=(complex a);

	complex &operator+=(complex a);
	complex &operator-=(complex a);
	complex &operator*=(complex a);
	complex &operator/=(complex a);

	complex &operator+=(float a);
	complex &operator-=(float a);
	complex &operator*=(float a);
	complex &operator/=(float a);
};

complex operator+(complex a, complex b);
complex operator-(complex a, complex b);
complex operator*(complex a, complex b);
complex operator/(complex a, complex b);

complex operator+(complex a, float b);
complex operator-(complex a, float b);
complex operator*(complex a, float b);
complex operator/(complex a, float b);

complex operator+(float a, complex b);
complex operator-(float a, complex b);
complex operator*(float a, complex b);
complex operator/(float a, complex b);

complex conjugate(complex a);
float magnitude(complex a);

#endif
