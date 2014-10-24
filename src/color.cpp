#include "color.h"
#include "standard.h"

rgb_t::rgb_t()
{
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
}

rgb_t::rgb_t(float i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
}

rgb_t::rgb_t(float r, float g, float b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
}

rgb_t::~rgb_t()
{
}

rgb_t &rgb_t::operator=(rgb_t a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_t &rgb_t::operator=(rgba_t a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_t &rgb_t::operator+=(rgb_t a)
{
	*this = *this + a;
	return *this;
}

rgb_t &rgb_t::operator-=(rgb_t a)
{
	*this = *this - a;
	return *this;
}

rgb_t &rgb_t::operator*=(rgb_t a)
{
	*this = *this * a;
	return *this;
}

rgb_t &rgb_t::operator/=(rgb_t a)
{
	*this = *this / a;
	return *this;
}

rgb_t &rgb_t::operator+=(rgba_t a)
{
	*this = *this + a;
	return *this;
}

rgb_t &rgb_t::operator-=(rgba_t a)
{
	*this = *this - a;
	return *this;
}

rgb_t &rgb_t::operator*=(rgba_t a)
{
	*this = *this * a;
	return *this;
}

rgb_t &rgb_t::operator/=(rgba_t a)
{
	*this = *this / a;
	return *this;
}

rgb_t &rgb_t::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgb_t &rgb_t::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgb_t &rgb_t::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgb_t &rgb_t::operator/=(float a)
{
	*this = *this / a;
	return *this;
}

float rgb_t::operator[](int i)
{
	switch (i)
	{
		case 0:
			return r;
			break;
		case 1:
			return g;
			break;
		case 2:
			return b;
			break;
		default:
			printf("Error: Out of Bounds");
			return 0.0;
			break;
	}
}

rgb_t operator+(rgb_t a, rgb_t b)
{
	return rgb_t(a.r + b.r, a.g + b.g, a.b + b.b);
}

rgb_t operator-(rgb_t a, rgb_t b)
{
	return rgb_t(a.r - b.r, a.g - b.g, a.b - b.b);
}

rgb_t operator*(rgb_t a, rgb_t b)
{
	return rgb_t(a.r * b.r, a.g * b.g, a.b * b.b);
}

rgb_t operator/(rgb_t a, rgb_t b)
{
	return rgb_t(a.r / b.r, a.g / b.g, a.b / b.b);
}

rgb_t operator+(rgb_t a, float b)
{
	return rgb_t(a.r + b, a.g + b, a.b + b);
}

rgb_t operator-(rgb_t a, float b)
{
	return rgb_t(a.r - b, a.g - b, a.b - b);
}

rgb_t operator*(rgb_t a, float b)
{
	return rgb_t(a.r * b, a.g * b, a.b * b);
}

rgb_t operator/(rgb_t a, float b)
{
	return rgb_t(a.r / b, a.g / b, a.b / b);
}

rgb_t operator+(float a, rgb_t b)
{
	return rgb_t(a + b.r, a + b.g, a + b.b);
}

rgb_t operator-(float a, rgb_t b)
{
	return rgb_t(a - b.r, a - b.g, a - b.b);
}

rgb_t operator*(float a, rgb_t b)
{
	return rgb_t(a * b.r, a * b.g, a * b.b);
}

rgb_t operator/(float a, rgb_t b)
{
	return rgb_t(a / b.r, a / b.g, a / b.b);
}

rgba_t::rgba_t()
{
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
	this->a = 0.0;
}

rgba_t::rgba_t(float i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = 1.0;
}

rgba_t::rgba_t(float i, float a) // greyscale instensity, alpha
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = a;
}

rgba_t::rgba_t(float r, float g, float b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0;
}

rgba_t::rgba_t(float r, float g, float b, float a) // red, blue, green, alpha
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

rgba_t::~rgba_t()
{
}

rgba_t &rgba_t::operator=(rgba_t a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = a.a;
	return *this;
}

rgba_t &rgba_t::operator=(rgb_t a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = 1.0;
	return *this;
}

rgba_t &rgba_t::operator+=(rgba_t a)
{
	*this = *this + a;
	return *this;
}

rgba_t &rgba_t::operator-=(rgba_t a)
{
	*this = *this - a;
	return *this;
}

rgba_t &rgba_t::operator*=(rgba_t a)
{
	*this = *this * a;
	return *this;
}

rgba_t &rgba_t::operator/=(rgba_t a)
{
	*this = *this / a;
	return *this;
}

rgba_t &rgba_t::operator+=(rgb_t a)
{
	*this = *this + a;
	return *this;
}

rgba_t &rgba_t::operator-=(rgb_t a)
{
	*this = *this - a;
	return *this;
}

rgba_t &rgba_t::operator*=(rgb_t a)
{
	*this = *this * a;
	return *this;
}

rgba_t &rgba_t::operator/=(rgb_t a)
{
	*this = *this / a;
	return *this;
}

rgba_t &rgba_t::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgba_t &rgba_t::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgba_t &rgba_t::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgba_t &rgba_t::operator/=(float a)
{
	*this = *this / a;
	return *this;
}
float rgba_t::operator[](int i)
{
	switch (i)
	{
		case 0:
			return r;
			break;
		case 1:
			return g;
			break;
		case 2:
			return b;
			break;
		case 3:
			return a;
			break;
		default:
			printf("Error: Out of Bounds");
			return 0.0;
			break;
	}
}

rgba_t operator+(rgba_t a, rgba_t b)
{
	return rgba_t(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

rgba_t operator-(rgba_t a, rgba_t b)
{
	return rgba_t(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}

rgba_t operator*(rgba_t a, rgba_t b)
{
	return rgba_t(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
}

rgba_t operator/(rgba_t a, rgba_t b)
{
	return rgba_t(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
}

rgba_t operator+(rgb_t a, rgba_t b)
{
	return rgba_t(a.r + b.r, a.g + b.g, a.b + b.b, 1.0);
}

rgba_t operator-(rgb_t a, rgba_t b)
{
	return rgba_t(a.r - b.r, a.g - b.g, a.b - b.b, 1.0 - b.a);
}

rgba_t operator*(rgb_t a, rgba_t b)
{
	return rgba_t(a.r * b.r, a.g * b.g, a.b * b.b, b.a);
}

rgba_t operator/(rgb_t a, rgba_t b)
{
	return rgba_t(a.r / b.r, a.g / b.g, a.b / b.b, 1.0 / b.a);
}

rgba_t operator+(rgba_t a, rgb_t b)
{
	return rgba_t(a.r + b.r, a.g + b.g, a.b + b.b, 1.0);
}

rgba_t operator-(rgba_t a, rgb_t b)
{
	return rgba_t(a.r - b.r, a.g - b.g, a.b - b.b, a.a - 1.0);
}

rgba_t operator*(rgba_t a, rgb_t b)
{
	return rgba_t(a.r * b.r, a.g * b.g, a.b * b.b, a.a);
}

rgba_t operator/(rgba_t a, rgb_t b)
{
	return rgba_t(a.r / b.r, a.g / b.g, a.b / b.b, a.a);
}

rgba_t operator+(rgba_t a, float b)
{
	return rgba_t(a.r + b, a.g + b, a.b + b, a.a + b);
}

rgba_t operator-(rgba_t a, float b)
{
	return rgba_t(a.r - b, a.g - b, a.b - b, a.a - b);
}

rgba_t operator*(rgba_t a, float b)
{
	return rgba_t(a.r * b, a.g * b, a.b * b, a.a * b);
}

rgba_t operator/(rgba_t a, float b)
{
	return rgba_t(a.r / b, a.g / b, a.b / b, a.a / b);
}

rgba_t operator+(float a, rgba_t b)
{
	return rgba_t(a + b.r, a + b.g, a + b.b, a + b.a);
}

rgba_t operator-(float a, rgba_t b)
{
	return rgba_t(a - b.r, a - b.g, a - b.b, a - b.a);
}

rgba_t operator*(float a, rgba_t b)
{
	return rgba_t(a * b.r, a * b.g, a * b.b, a * b.a);
}

rgba_t operator/(float a, rgba_t b)
{
	return rgba_t(a / b.r, a / b.g, a / b.b, a / b.a);
}
