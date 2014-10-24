#include "color.h"

/*******************
 *     rgb_hdr     *
 *******************/
rgb_hdr::rgb_hdr()
{
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
}

rgb_hdr::rgb_hdr(float i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
}

rgb_hdr::rgb_hdr(float r, float g, float b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
}

rgb_hdr::~rgb_hdr()
{
}

rgb_hdr &rgb_hdr::operator=(rgb_hdr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_hdr &rgb_hdr::operator=(rgba_hdr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_hdr &rgb_hdr::operator=(rgb_ldr a)
{
	this->r = ((float)a.r)/255.0;
	this->g = ((float)a.g)/255.0;
	this->b = ((float)a.b)/255.0;
	return *this;
}

rgb_hdr &rgb_hdr::operator=(rgba_ldr a)
{
	this->r = ((float)a.r)/255.0;
	this->g = ((float)a.g)/255.0;
	this->b = ((float)a.b)/255.0;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(rgb_hdr a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(rgb_hdr a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(rgb_hdr a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(rgb_hdr a)
{
	*this = *this / a;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(rgba_hdr a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(rgba_hdr a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(rgba_hdr a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(rgba_hdr a)
{
	*this = *this / a;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(rgb_ldr a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(rgb_ldr a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(rgb_ldr a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(rgb_ldr a)
{
	*this = *this / a;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(rgba_ldr a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(rgba_ldr a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(rgba_ldr a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(rgba_ldr a)
{
	*this = *this / a;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(float a)
{
	*this = *this / a;
	return *this;
}

rgb_hdr &rgb_hdr::operator+=(unsigned char a)
{
	*this = *this + a;
	return *this;
}

rgb_hdr &rgb_hdr::operator-=(unsigned char a)
{
	*this = *this - a;
	return *this;
}

rgb_hdr &rgb_hdr::operator*=(unsigned char a)
{
	*this = *this * a;
	return *this;
}

rgb_hdr &rgb_hdr::operator/=(unsigned char a)
{
	*this = *this / a;
	return *this;
}

float rgb_hdr::operator[](int i)
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
			return 0.0;
			break;
	}
}

rgb_hdr operator+(rgb_hdr a, rgb_hdr b)
{
	return rgb_hdr(a.r + b.r, a.g + b.g, a.b + b.b);
}

rgb_hdr operator-(rgb_hdr a, rgb_hdr b)
{
	return rgb_hdr(a.r - b.r, a.g - b.g, a.b - b.b);
}

rgb_hdr operator*(rgb_hdr a, rgb_hdr b)
{
	return rgb_hdr(a.r * b.r, a.g * b.g, a.b * b.b);
}

rgb_hdr operator/(rgb_hdr a, rgb_hdr b)
{
	return rgb_hdr(a.r / b.r, a.g / b.g, a.b / b.b);
}

rgb_hdr operator+(rgb_hdr a, rgba_hdr b)
{
	return rgb_hdr(a.r + b.r, a.g + b.g, a.b + b.b);
}

rgb_hdr operator-(rgb_hdr a, rgba_hdr b)
{
	return rgb_hdr(a.r - b.r, a.g - b.g, a.b - b.b);
}

rgb_hdr operator*(rgb_hdr a, rgba_hdr b)
{
	return rgb_hdr(a.r * b.r, a.g * b.g, a.b * b.b);
}

rgb_hdr operator/(rgb_hdr a, rgba_hdr b)
{
	return rgb_hdr(a.r / b.r, a.g / b.g, a.b / b.b);
}

rgb_hdr operator+(rgb_hdr a, rgb_ldr b)
{
	return rgb_hdr(a.r + (float(b.r)/255.0), a.g + (float(b.g)/255.0), a.b + (float(b.b)/255.0));
}

rgb_hdr operator-(rgb_hdr a, rgb_ldr b)
{
	return rgb_hdr(a.r - (float(b.r)/255.0), a.g - (float(b.g)/255.0), a.b - (float(b.b)/255.0));
}

rgb_hdr operator*(rgb_hdr a, rgb_ldr b)
{
	return rgb_hdr(a.r * (float(b.r)/255.0), a.g * (float(b.g)/255.0), a.b * (float(b.b)/255.0));
}

rgb_hdr operator/(rgb_hdr a, rgb_ldr b)
{
	return rgb_hdr(a.r / (float(b.r)/255.0), a.g / (float(b.g)/255.0), a.b / (float(b.b)/255.0));
}

rgb_hdr operator+(rgb_hdr a, rgba_ldr b)
{
	return rgb_hdr(a.r + (float(b.r)/255.0), a.g + (float(b.g)/255.0), a.b + (float(b.b)/255.0));
}

rgb_hdr operator-(rgb_hdr a, rgba_ldr b)
{
	return rgb_hdr(a.r - (float(b.r)/255.0), a.g - (float(b.g)/255.0), a.b - (float(b.b)/255.0));
}

rgb_hdr operator*(rgb_hdr a, rgba_ldr b)
{
	return rgb_hdr(a.r * (float(b.r)/255.0), a.g * (float(b.g)/255.0), a.b * (float(b.b)/255.0));
}

rgb_hdr operator/(rgb_hdr a, rgba_ldr b)
{
	return rgb_hdr(a.r / (float(b.r)/255.0), a.g / (float(b.g)/255.0), a.b / (float(b.b)/255.0));
}

rgb_hdr operator+(rgb_hdr a, float b)
{
	return rgb_hdr(a.r + b, a.g + b, a.b + b);
}

rgb_hdr operator-(rgb_hdr a, float b)
{
	return rgb_hdr(a.r - b, a.g - b, a.b - b);
}

rgb_hdr operator*(rgb_hdr a, float b)
{
	return rgb_hdr(a.r * b, a.g * b, a.b * b);
}

rgb_hdr operator/(rgb_hdr a, float b)
{
	return rgb_hdr(a.r / b, a.g / b, a.b / b);
}

rgb_hdr operator+(float a, rgb_hdr b)
{
	return rgb_hdr(a + b.r, a + b.g, a + b.b);
}

rgb_hdr operator-(float a, rgb_hdr b)
{
	return rgb_hdr(a - b.r, a - b.g, a - b.b);
}

rgb_hdr operator*(float a, rgb_hdr b)
{
	return rgb_hdr(a * b.r, a * b.g, a * b.b);
}

rgb_hdr operator/(float a, rgb_hdr b)
{
	return rgb_hdr(a / b.r, a / b.g, a / b.b);
}

rgb_hdr operator+(rgb_hdr a, unsigned char b)
{
	return rgb_hdr(a.r + (float(b)/255.0), a.g + (float(b)/255.0), a.b + (float(b)/255.0));
}

rgb_hdr operator-(rgb_hdr a, unsigned char b)
{
	return rgb_hdr(a.r - (float(b)/255.0), a.g - (float(b)/255.0), a.b - (float(b)/255.0));
}

rgb_hdr operator*(rgb_hdr a, unsigned char b)
{
	return rgb_hdr(a.r * (float(b)/255.0), a.g * (float(b)/255.0), a.b * (float(b)/255.0));
}

rgb_hdr operator/(rgb_hdr a, unsigned char b)
{
	return rgb_hdr(a.r / (float(b)/255.0), a.g / (float(b)/255.0), a.b / (float(b)/255.0));
}

rgb_hdr operator+(unsigned char a, rgb_hdr b)
{
	return rgb_hdr((float(a)/255.0) + b.r, (float(a)/255.0) + b.g, (float(a)/255.0) + b.b);
}

rgb_hdr operator-(unsigned char a, rgb_hdr b)
{
	return rgb_hdr((float(a)/255.0) - b.r, (float(a)/255.0) - b.g, (float(a)/255.0) - b.b);
}

rgb_hdr operator*(unsigned char a, rgb_hdr b)
{
	return rgb_hdr((float(a)/255.0) * b.r, (float(a)/255.0) * b.g, (float(a)/255.0) * b.b);
}

rgb_hdr operator/(unsigned char a, rgb_hdr b)
{
	return rgb_hdr((float(a)/255.0) / b.r, (float(a)/255.0) / b.g, (float(a)/255.0) / b.b);
}

/********************
 *     rgba_hdr     *
 ********************/
rgba_hdr::rgba_hdr()
{
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
	this->a = 0.0;
}

rgba_hdr::rgba_hdr(float i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = 1.0;
}

rgba_hdr::rgba_hdr(float i, float a) // greyscale instensity, alpha
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = a;
}

rgba_hdr::rgba_hdr(float r, float g, float b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0;
}

rgba_hdr::rgba_hdr(float r, float g, float b, float a) // red, blue, green, alpha
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

rgba_hdr::~rgba_hdr()
{
}

rgba_hdr &rgba_hdr::operator=(rgba_hdr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = a.a;
	return *this;
}

rgba_hdr &rgba_hdr::operator=(rgb_hdr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = 1.0;
	return *this;
}

rgba_hdr &rgba_hdr::operator=(rgba_ldr a)
{
	this->r = (float)a.r/255.0;
	this->g = (float)a.g/255.0;
	this->b = (float)a.b/255.0;
	this->a = (float)a.a/255.0;
	return *this;
}

rgba_hdr &rgba_hdr::operator=(rgb_ldr a)
{
	this->r = (float)a.r/255.0;
	this->g = (float)a.g/255.0;
	this->b = (float)a.b/255.0;
	this->a = 1.0;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(rgba_hdr a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(rgba_hdr a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(rgba_hdr a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(rgba_hdr a)
{
	*this = *this / a;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(rgb_hdr a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(rgb_hdr a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(rgb_hdr a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(rgb_hdr a)
{
	*this = *this / a;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(rgba_ldr a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(rgba_ldr a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(rgba_ldr a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(rgba_ldr a)
{
	*this = *this / a;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(rgb_ldr a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(rgb_ldr a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(rgb_ldr a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(rgb_ldr a)
{
	*this = *this / a;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(float a)
{
	*this = *this / a;
	return *this;
}

rgba_hdr &rgba_hdr::operator+=(unsigned char a)
{
	*this = *this + a;
	return *this;
}

rgba_hdr &rgba_hdr::operator-=(unsigned char a)
{
	*this = *this - a;
	return *this;
}

rgba_hdr &rgba_hdr::operator*=(unsigned char a)
{
	*this = *this * a;
	return *this;
}

rgba_hdr &rgba_hdr::operator/=(unsigned char a)
{
	*this = *this / a;
	return *this;
}

float rgba_hdr::operator[](int i)
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
			return 0.0;
			break;
	}
}

rgba_hdr operator+(rgba_hdr a, rgba_hdr b)
{
	return rgba_hdr(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

rgba_hdr operator-(rgba_hdr a, rgba_hdr b)
{
	return rgba_hdr(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}

rgba_hdr operator*(rgba_hdr a, rgba_hdr b)
{
	return rgba_hdr(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
}

rgba_hdr operator/(rgba_hdr a, rgba_hdr b)
{
	return rgba_hdr(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
}

rgba_hdr operator+(rgba_hdr a, rgb_hdr b)
{
	return rgba_hdr(a.r + b.r, a.g + b.g, a.b + b.b, 1.0);
}

rgba_hdr operator-(rgba_hdr a, rgb_hdr b)
{
	return rgba_hdr(a.r - b.r, a.g - b.g, a.b - b.b, a.a - 1.0);
}

rgba_hdr operator*(rgba_hdr a, rgb_hdr b)
{
	return rgba_hdr(a.r * b.r, a.g * b.g, a.b * b.b, a.a);
}

rgba_hdr operator/(rgba_hdr a, rgb_hdr b)
{
	return rgba_hdr(a.r / b.r, a.g / b.g, a.b / b.b, a.a);
}

rgba_hdr operator+(rgba_hdr a, rgba_ldr b)
{
	return rgba_hdr(a.r + (float(b.r)/255.0), a.g + (float(b.g)/255.0), a.b + (float(b.b)/255.0), a.a + (float(b.a)/255.0));
}

rgba_hdr operator-(rgba_hdr a, rgba_ldr b)
{
	return rgba_hdr(a.r - (float(b.r)/255.0), a.g - (float(b.g)/255.0), a.b - (float(b.b)/255.0), a.a - (float(b.a)/255.0));
}

rgba_hdr operator*(rgba_hdr a, rgba_ldr b)
{
	return rgba_hdr(a.r * (float(b.r)/255.0), a.g * (float(b.g)/255.0), a.b * (float(b.b)/255.0), a.a * (float(b.a)/255.0));
}

rgba_hdr operator/(rgba_hdr a, rgba_ldr b)
{
	return rgba_hdr(a.r / (float(b.r)/255.0), a.g / (float(b.g)/255.0), a.b / (float(b.b)/255.0), a.a / (float(b.a)/255.0));
}

rgba_hdr operator+(rgba_hdr a, rgb_ldr b)
{
	return rgba_hdr(a.r + (float(b.r)/255.0), a.g + (float(b.g)/255.0), a.b + (float(b.b)/255.0), 1.0);
}

rgba_hdr operator-(rgba_hdr a, rgb_ldr b)
{
	return rgba_hdr(a.r - (float(b.r)/255.0), a.g - (float(b.g)/255.0), a.b - (float(b.b)/255.0), a.a - 1.0);
}

rgba_hdr operator*(rgba_hdr a, rgb_ldr b)
{
	return rgba_hdr(a.r * (float(b.r)/255.0), a.g * (float(b.g)/255.0), a.b * (float(b.b)/255.0), a.a);
}

rgba_hdr operator/(rgba_hdr a, rgb_ldr b)
{
	return rgba_hdr(a.r / (float(b.r)/255.0), a.g / (float(b.g)/255.0), a.b / (float(b.b)/255.0), a.a);
}

rgba_hdr operator+(rgba_hdr a, float b)
{
	return rgba_hdr(a.r + b, a.g + b, a.b + b, a.a + b);
}

rgba_hdr operator-(rgba_hdr a, float b)
{
	return rgba_hdr(a.r - b, a.g - b, a.b - b, a.a - b);
}

rgba_hdr operator*(rgba_hdr a, float b)
{
	return rgba_hdr(a.r * b, a.g * b, a.b * b, a.a * b);
}

rgba_hdr operator/(rgba_hdr a, float b)
{
	return rgba_hdr(a.r / b, a.g / b, a.b / b, a.a / b);
}

rgba_hdr operator+(float a, rgba_hdr b)
{
	return rgba_hdr(a + b.r, a + b.g, a + b.b, a + b.a);
}

rgba_hdr operator-(float a, rgba_hdr b)
{
	return rgba_hdr(a - b.r, a - b.g, a - b.b, a - b.a);
}

rgba_hdr operator*(float a, rgba_hdr b)
{
	return rgba_hdr(a * b.r, a * b.g, a * b.b, a * b.a);
}

rgba_hdr operator/(float a, rgba_hdr b)
{
	return rgba_hdr(a / b.r, a / b.g, a / b.b, a / b.a);
}

rgba_hdr operator+(rgba_hdr a, unsigned char b)
{
	return rgba_hdr(a.r + (float(b)/255.0), a.g + (float(b)/255.0), a.b + (float(b)/255.0), a.a + (float(b)/255.0));
}

rgba_hdr operator-(rgba_hdr a, unsigned char b)
{
	return rgba_hdr(a.r - (float(b)/255.0), a.g - (float(b)/255.0), a.b - (float(b)/255.0), a.a - (float(b)/255.0));
}

rgba_hdr operator*(rgba_hdr a, unsigned char b)
{
	return rgba_hdr(a.r * (float(b)/255.0), a.g * (float(b)/255.0), a.b * (float(b)/255.0), a.a * (float(b)/255.0));
}

rgba_hdr operator/(rgba_hdr a, unsigned char b)
{
	return rgba_hdr(a.r / (float(b)/255.0), a.g / (float(b)/255.0), a.b / (float(b)/255.0), a.a / (float(b)/255.0));
}

rgba_hdr operator+(unsigned char a, rgba_hdr b)
{
	return rgba_hdr((float(a)/255.0) + b.r, (float(a)/255.0) + b.g, (float(a)/255.0) + b.b, (float(a)/255.0) + b.a);
}

rgba_hdr operator-(unsigned char a, rgba_hdr b)
{
	return rgba_hdr((float(a)/255.0) - b.r, (float(a)/255.0) - b.g, (float(a)/255.0) - b.b, (float(a)/255.0) - b.a);
}

rgba_hdr operator*(unsigned char a, rgba_hdr b)
{
	return rgba_hdr((float(a)/255.0) * b.r, (float(a)/255.0) * b.g, (float(a)/255.0) * b.b, (float(a)/255.0) * b.a);
}

rgba_hdr operator/(unsigned char a, rgba_hdr b)
{
	return rgba_hdr((float(a)/255.0) / b.r, (float(a)/255.0) / b.g, (float(a)/255.0) / b.b, (float(a)/255.0) / b.a);
}

/*******************
 *     rgb_ldr     *
 *******************/
rgb_ldr::rgb_ldr()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

rgb_ldr::rgb_ldr(unsigned char i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
}

rgb_ldr::rgb_ldr(unsigned char r, unsigned char g, unsigned char b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
}

rgb_ldr::~rgb_ldr()
{
}

rgb_ldr &rgb_ldr::operator=(rgb_ldr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_ldr &rgb_ldr::operator=(rgba_ldr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	return *this;
}

rgb_ldr &rgb_ldr::operator=(rgb_hdr a)
{
	this->r = ((unsigned char)(a.r*255.0));
	this->g = ((unsigned char)(a.g*255.0));
	this->b = ((unsigned char)(a.b*255.0));
	return *this;
}

rgb_ldr &rgb_ldr::operator=(rgba_hdr a)
{
	this->r = ((unsigned char)(a.r*255.0));
	this->g = ((unsigned char)(a.g*255.0));
	this->b = ((unsigned char)(a.b*255.0));
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(rgb_ldr a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(rgb_ldr a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(rgb_ldr a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(rgb_ldr a)
{
	*this = *this / a;
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(rgba_ldr a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(rgba_ldr a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(rgba_ldr a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(rgba_ldr a)
{
	*this = *this / a;
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(rgb_hdr a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(rgb_hdr a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(rgb_hdr a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(rgb_hdr a)
{
	*this = *this / a;
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(rgba_hdr a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(rgba_hdr a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(rgba_hdr a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(rgba_hdr a)
{
	*this = *this / a;
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(float a)
{
	*this = *this / a;
	return *this;
}

rgb_ldr &rgb_ldr::operator+=(unsigned char a)
{
	*this = *this + a;
	return *this;
}

rgb_ldr &rgb_ldr::operator-=(unsigned char a)
{
	*this = *this - a;
	return *this;
}

rgb_ldr &rgb_ldr::operator*=(unsigned char a)
{
	*this = *this * a;
	return *this;
}

rgb_ldr &rgb_ldr::operator/=(unsigned char a)
{
	*this = *this / a;
	return *this;
}

unsigned char rgb_ldr::operator[](int i)
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
			return 0;
			break;
	}
}

rgb_ldr operator+(rgb_ldr a, rgb_ldr b)
{
	return rgb_ldr(a.r + b.r, a.g + b.g, a.b + b.b);
}

rgb_ldr operator-(rgb_ldr a, rgb_ldr b)
{
	return rgb_ldr(a.r - b.r, a.g - b.g, a.b - b.b);
}

rgb_ldr operator*(rgb_ldr a, rgb_ldr b)
{
	return rgb_ldr(a.r * b.r, a.g * b.g, a.b * b.b);
}

rgb_ldr operator/(rgb_ldr a, rgb_ldr b)
{
	return rgb_ldr(a.r / b.r, a.g / b.g, a.b / b.b);
}

rgb_ldr operator+(rgb_ldr a, rgba_ldr b)
{
	return rgb_ldr(a.r + b.r, a.g + b.g, a.b + b.b);
}

rgb_ldr operator-(rgb_ldr a, rgba_ldr b)
{
	return rgb_ldr(a.r - b.r, a.g - b.g, a.b - b.b);
}

rgb_ldr operator*(rgb_ldr a, rgba_ldr b)
{
	return rgb_ldr(a.r * b.r, a.g * b.g, a.b * b.b);
}

rgb_ldr operator/(rgb_ldr a, rgba_ldr b)
{
	return rgb_ldr(a.r / b.r, a.g / b.g, a.b / b.b);
}

rgb_ldr operator+(rgb_ldr a, rgb_hdr b)
{
	return rgb_ldr(a.r + ((unsigned char)(b.r*255.0)), a.g + ((unsigned char)(b.g*255.0)), a.b + ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator-(rgb_ldr a, rgb_hdr b)
{
	return rgb_ldr(a.r - ((unsigned char)(b.r*255.0)), a.g - ((unsigned char)(b.g*255.0)), a.b - ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator*(rgb_ldr a, rgb_hdr b)
{
	return rgb_ldr(a.r * ((unsigned char)(b.r*255.0)), a.g * ((unsigned char)(b.g*255.0)), a.b * ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator/(rgb_ldr a, rgb_hdr b)
{
	return rgb_ldr(a.r / ((unsigned char)(b.r*255.0)), a.g / ((unsigned char)(b.g*255.0)), a.b / ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator+(rgb_ldr a, rgba_hdr b)
{
	return rgb_ldr(a.r + ((unsigned char)(b.r*255.0)), a.g + ((unsigned char)(b.g*255.0)), a.b + ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator-(rgb_ldr a, rgba_hdr b)
{
	return rgb_ldr(a.r - ((unsigned char)(b.r*255.0)), a.g - ((unsigned char)(b.g*255.0)), a.b - ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator*(rgb_ldr a, rgba_hdr b)
{
	return rgb_ldr(a.r * ((unsigned char)(b.r*255.0)), a.g * ((unsigned char)(b.g*255.0)), a.b * ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator/(rgb_ldr a, rgba_hdr b)
{
	return rgb_ldr(a.r / ((unsigned char)(b.r*255.0)), a.g / ((unsigned char)(b.g*255.0)), a.b / ((unsigned char)(b.b*255.0)));
}

rgb_ldr operator+(rgb_ldr a, float b)
{
	return rgb_ldr(a.r + ((unsigned char)(b*255.0)), a.g + ((unsigned char)(b*255.0)), a.b + ((unsigned char)(b*255.0)));
}

rgb_ldr operator-(rgb_ldr a, float b)
{
	return rgb_ldr(a.r - ((unsigned char)(b*255.0)), a.g - ((unsigned char)(b*255.0)), a.b - ((unsigned char)(b*255.0)));
}

rgb_ldr operator*(rgb_ldr a, float b)
{
	return rgb_ldr(a.r * ((unsigned char)(b*255.0)), a.g * ((unsigned char)(b*255.0)), a.b * ((unsigned char)(b*255.0)));
}

rgb_ldr operator/(rgb_ldr a, float b)
{
	return rgb_ldr(a.r / ((unsigned char)(b*255.0)), a.g / ((unsigned char)(b*255.0)), a.b / ((unsigned char)(b*255.0)));
}

rgb_ldr operator+(float a, rgb_ldr b)
{
	return rgb_ldr(((unsigned char)(a*255.0)) + b.r, ((unsigned char)(a*255.0)) + b.g, ((unsigned char)(a*255.0)) + b.b);
}

rgb_ldr operator-(float a, rgb_ldr b)
{
	return rgb_ldr(((unsigned char)(a*255.0)) - b.r, ((unsigned char)(a*255.0)) - b.g, ((unsigned char)(a*255.0)) - b.b);
}

rgb_ldr operator*(float a, rgb_ldr b)
{
	return rgb_ldr(((unsigned char)(a*255.0)) * b.r, ((unsigned char)(a*255.0)) * b.g, ((unsigned char)(a*255.0)) * b.b);
}

rgb_ldr operator/(float a, rgb_ldr b)
{
	return rgb_ldr(((unsigned char)(a*255.0)) / b.r, ((unsigned char)(a*255.0)) / b.g, ((unsigned char)(a*255.0)) / b.b);
}

rgb_ldr operator+(rgb_ldr a, unsigned char b)
{
	return rgb_ldr(a.r + b, a.g + b, a.b + b);
}

rgb_ldr operator-(rgb_ldr a, unsigned char b)
{
	return rgb_ldr(a.r - b, a.g - b, a.b - b);
}

rgb_ldr operator*(rgb_ldr a, unsigned char b)
{
	return rgb_ldr(a.r * b, a.g * b, a.b * b);
}

rgb_ldr operator/(rgb_ldr a, unsigned char b)
{
	return rgb_ldr(a.r / b, a.g / b, a.b / b);
}

rgb_ldr operator+(unsigned char a, rgb_ldr b)
{
	return rgb_ldr(a + b.r, a + b.g, a + b.b);
}

rgb_ldr operator-(unsigned char a, rgb_ldr b)
{
	return rgb_ldr(a - b.r, a - b.g, a - b.b);
}

rgb_ldr operator*(unsigned char a, rgb_ldr b)
{
	return rgb_ldr(a * b.r, a * b.g, a * b.b);
}

rgb_ldr operator/(unsigned char a, rgb_ldr b)
{
	return rgb_ldr(a / b.r, a / b.g, a / b.b);
}

/********************
 *     rgba_ldr     *
 ********************/
rgba_ldr::rgba_ldr()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 0;
}

rgba_ldr::rgba_ldr(unsigned char i) // greyscale intensity
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = 255;
}

rgba_ldr::rgba_ldr(unsigned char i, unsigned char a) // greyscale instensity, alpha
{
	this->r = i;
	this->g = i;
	this->b = i;
	this->a = a;
}

rgba_ldr::rgba_ldr(unsigned char r, unsigned char g, unsigned char b) // red, blue, green
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}

rgba_ldr::rgba_ldr(unsigned char r, unsigned char g, unsigned char b, unsigned char a) // red, blue, green, alpha
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

rgba_ldr::~rgba_ldr()
{
}

rgba_ldr &rgba_ldr::operator=(rgba_ldr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = a.a;
	return *this;
}

rgba_ldr &rgba_ldr::operator=(rgb_ldr a)
{
	this->r = a.r;
	this->g = a.g;
	this->b = a.b;
	this->a = 255;
	return *this;
}

rgba_ldr &rgba_ldr::operator=(rgba_hdr a)
{
	this->r = (unsigned char)(a.r*255.0);
	this->g = (unsigned char)(a.g*255.0);
	this->b = (unsigned char)(a.b*255.0);
	this->a = (unsigned char)(a.a*255.0);
	return *this;
}

rgba_ldr &rgba_ldr::operator=(rgb_hdr a)
{
	this->r = (unsigned char)(a.r*255.0);
	this->g = (unsigned char)(a.g*255.0);
	this->b = (unsigned char)(a.b*255.0);
	this->a = 255;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(rgba_ldr a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(rgba_ldr a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(rgba_ldr a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(rgba_ldr a)
{
	*this = *this / a;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(rgb_ldr a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(rgb_ldr a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(rgb_ldr a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(rgb_ldr a)
{
	*this = *this / a;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(rgba_hdr a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(rgba_hdr a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(rgba_hdr a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(rgba_hdr a)
{
	*this = *this / a;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(rgb_hdr a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(rgb_hdr a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(rgb_hdr a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(rgb_hdr a)
{
	*this = *this / a;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(float a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(float a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(float a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(float a)
{
	*this = *this / a;
	return *this;
}

rgba_ldr &rgba_ldr::operator+=(unsigned char a)
{
	*this = *this + a;
	return *this;
}

rgba_ldr &rgba_ldr::operator-=(unsigned char a)
{
	*this = *this - a;
	return *this;
}

rgba_ldr &rgba_ldr::operator*=(unsigned char a)
{
	*this = *this * a;
	return *this;
}

rgba_ldr &rgba_ldr::operator/=(unsigned char a)
{
	*this = *this / a;
	return *this;
}

unsigned char rgba_ldr::operator[](int i)
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
			return 0;
			break;
	}
}

rgba_ldr operator+(rgba_ldr a, rgba_ldr b)
{
	return rgba_ldr(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

rgba_ldr operator-(rgba_ldr a, rgba_ldr b)
{
	return rgba_ldr(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}

rgba_ldr operator*(rgba_ldr a, rgba_ldr b)
{
	return rgba_ldr(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
}

rgba_ldr operator/(rgba_ldr a, rgba_ldr b)
{
	return rgba_ldr(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
}

rgba_ldr operator+(rgba_ldr a, rgb_ldr b)
{
	return rgba_ldr(a.r + b.r, a.g + b.g, a.b + b.b, 255);
}

rgba_ldr operator-(rgba_ldr a, rgb_ldr b)
{
	return rgba_ldr(a.r - b.r, a.g - b.g, a.b - b.b, a.a - 255);
}

rgba_ldr operator*(rgba_ldr a, rgb_ldr b)
{
	return rgba_ldr(a.r * b.r, a.g * b.g, a.b * b.b, a.a);
}

rgba_ldr operator/(rgba_ldr a, rgb_ldr b)
{
	return rgba_ldr(a.r / b.r, a.g / b.g, a.b / b.b, a.a);
}

rgba_ldr operator+(rgba_ldr a, rgba_hdr b)
{
	return rgba_ldr(a.r + ((unsigned char)(b.r*255.0)), a.g + ((unsigned char)(b.g*255.0)), a.b + ((unsigned char)(b.b*255.0)), a.a + ((unsigned char)(b.a*255.0)));
}

rgba_ldr operator-(rgba_ldr a, rgba_hdr b)
{
	return rgba_ldr(a.r - ((unsigned char)(b.r*255.0)), a.g - ((unsigned char)(b.g*255.0)), a.b - ((unsigned char)(b.b*255.0)), a.a - ((unsigned char)(b.a*255.0)));
}

rgba_ldr operator*(rgba_ldr a, rgba_hdr b)
{
	return rgba_ldr(a.r * ((unsigned char)(b.r*255.0)), a.g * ((unsigned char)(b.g*255.0)), a.b * ((unsigned char)(b.b*255.0)), a.a * ((unsigned char)(b.a*255.0)));
}

rgba_ldr operator/(rgba_ldr a, rgba_hdr b)
{
	return rgba_ldr(a.r / ((unsigned char)(b.r*255.0)), a.g / ((unsigned char)(b.g*255.0)), a.b / ((unsigned char)(b.b*255.0)), a.a / ((unsigned char)(b.a*255.0)));
}

rgba_ldr operator+(rgba_ldr a, rgb_hdr b)
{
	return rgba_ldr(a.r + ((unsigned char)(b.r*255.0)), a.g + ((unsigned char)(b.g*255.0)), a.b + ((unsigned char)(b.b*255.0)), 255);
}

rgba_ldr operator-(rgba_ldr a, rgb_hdr b)
{
	return rgba_ldr(a.r - ((unsigned char)(b.r*255.0)), a.g - ((unsigned char)(b.g*255.0)), a.b - ((unsigned char)(b.b*255.0)), a.a - 255);
}

rgba_ldr operator*(rgba_ldr a, rgb_hdr b)
{
	return rgba_ldr(a.r * ((unsigned char)(b.r*255.0)), a.g * ((unsigned char)(b.g*255.0)), a.b * ((unsigned char)(b.b*255.0)), a.a);
}

rgba_ldr operator/(rgba_ldr a, rgb_hdr b)
{
	return rgba_ldr(a.r / ((unsigned char)(b.r*255.0)), a.g / ((unsigned char)(b.g*255.0)), a.b / ((unsigned char)(b.b*255.0)), a.a);
}

rgba_ldr operator+(rgba_ldr a, float b)
{
	return rgba_ldr(a.r + ((unsigned char)(b*255.0)), a.g + ((unsigned char)(b*255.0)), a.b + ((unsigned char)(b*255.0)), a.a + ((unsigned char)(b*255.0)));
}

rgba_ldr operator-(rgba_ldr a, float b)
{
	return rgba_ldr(a.r - ((unsigned char)(b*255.0)), a.g - ((unsigned char)(b*255.0)), a.b - ((unsigned char)(b*255.0)), a.a - ((unsigned char)(b*255.0)));
}

rgba_ldr operator*(rgba_ldr a, float b)
{
	return rgba_ldr(a.r * ((unsigned char)(b*255.0)), a.g * ((unsigned char)(b*255.0)), a.b * ((unsigned char)(b*255.0)), a.a * ((unsigned char)(b*255.0)));
}

rgba_ldr operator/(rgba_ldr a, float b)
{
	return rgba_ldr(a.r / ((unsigned char)(b*255.0)), a.g / ((unsigned char)(b*255.0)), a.b / ((unsigned char)(b*255.0)), a.a / ((unsigned char)(b*255.0)));
}

rgba_ldr operator+(float a, rgba_ldr b)
{
	return rgba_ldr(((unsigned char)(a*255.0)) + b.r, ((unsigned char)(a*255.0)) + b.g, ((unsigned char)(a*255.0)) + b.b, ((unsigned char)(a*255.0)) + b.a);
}

rgba_ldr operator-(float a, rgba_ldr b)
{
	return rgba_ldr(((unsigned char)(a*255.0)) - b.r, ((unsigned char)(a*255.0)) - b.g, ((unsigned char)(a*255.0)) - b.b, ((unsigned char)(a*255.0)) - b.a);
}

rgba_ldr operator*(float a, rgba_ldr b)
{
	return rgba_ldr(((unsigned char)(a*255.0)) * b.r, ((unsigned char)(a*255.0)) * b.g, ((unsigned char)(a*255.0)) * b.b, ((unsigned char)(a*255.0)) * b.a);
}

rgba_ldr operator/(float a, rgba_ldr b)
{
	return rgba_ldr(((unsigned char)(a*255.0)) / b.r, ((unsigned char)(a*255.0)) / b.g, ((unsigned char)(a*255.0)) / b.b, ((unsigned char)(a*255.0)) / b.a);
}

rgba_ldr operator+(rgba_ldr a, unsigned char b)
{
	return rgba_ldr(a.r + b, a.g + b, a.b + b, a.a + b);
}

rgba_ldr operator-(rgba_ldr a, unsigned char b)
{
	return rgba_ldr(a.r - b, a.g - b, a.b - b, a.a - b);
}

rgba_ldr operator*(rgba_ldr a, unsigned char b)
{
	return rgba_ldr(a.r * b, a.g * b, a.b * b, a.a * b);
}

rgba_ldr operator/(rgba_ldr a, unsigned char b)
{
	return rgba_ldr(a.r / b, a.g / b, a.b / b, a.a / b);
}

rgba_ldr operator+(unsigned char a, rgba_ldr b)
{
	return rgba_ldr(a + b.r, a + b.g, a + b.b, a + b.a);
}

rgba_ldr operator-(unsigned char a, rgba_ldr b)
{
	return rgba_ldr(a - b.r, a - b.g, a - b.b, a - b.a);
}

rgba_ldr operator*(unsigned char a, rgba_ldr b)
{
	return rgba_ldr(a * b.r, a * b.g, a * b.b, a * b.a);
}

rgba_ldr operator/(unsigned char a, rgba_ldr b)
{
	return rgba_ldr(a / b.r, a / b.g, a / b.b, a / b.a);
}
