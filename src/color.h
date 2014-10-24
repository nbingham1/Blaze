#ifndef color_h
#define color_h

struct rgb_t; // red, green, blue
struct rgba_t; // red, green, blue, alpha

struct rgb_t
{
	rgb_t();
	rgb_t(float i); // greyscale intensity
	rgb_t(float r, float g, float b); // red, blue, green
	~rgb_t();

	float r;
	float g;
	float b;

	rgb_t &operator=(rgb_t a);
	rgb_t &operator=(rgba_t a);

	rgb_t &operator+=(rgb_t a);
	rgb_t &operator-=(rgb_t a);
	rgb_t &operator*=(rgb_t a);
	rgb_t &operator/=(rgb_t a);

	rgb_t &operator+=(rgba_t a);
	rgb_t &operator-=(rgba_t a);
	rgb_t &operator*=(rgba_t a);
	rgb_t &operator/=(rgba_t a);

	rgb_t &operator+=(float a);
	rgb_t &operator-=(float a);
	rgb_t &operator*=(float a);
	rgb_t &operator/=(float a);

	float operator[](int i);
};

rgb_t operator+(rgb_t a, rgb_t b);
rgb_t operator-(rgb_t a, rgb_t b);
rgb_t operator*(rgb_t a, rgb_t b);
rgb_t operator/(rgb_t a, rgb_t b);

rgb_t operator+(rgb_t a, float b);
rgb_t operator-(rgb_t a, float b);
rgb_t operator*(rgb_t a, float b);
rgb_t operator/(rgb_t a, float b);

rgb_t operator+(float a, rgb_t b);
rgb_t operator-(float a, rgb_t b);
rgb_t operator*(float a, rgb_t b);
rgb_t operator/(float a, rgb_t b);

struct rgba_t
{
	rgba_t();
	rgba_t(float i); // greyscale intensity
	rgba_t(float i, float a); // greyscale instensity, alpha
	rgba_t(float r, float g, float b); // red, blue, green
	rgba_t(float r, float g, float b, float a); // red, blue, green, alpha
	~rgba_t();

	float r;
	float g;
	float b;
	float a;

	rgba_t &operator=(rgba_t a);
	rgba_t &operator=(rgb_t a);

	rgba_t &operator+=(rgba_t a);
	rgba_t &operator-=(rgba_t a);
	rgba_t &operator*=(rgba_t a);
	rgba_t &operator/=(rgba_t a);

	rgba_t &operator+=(rgb_t a);
	rgba_t &operator-=(rgb_t a);
	rgba_t &operator*=(rgb_t a);
	rgba_t &operator/=(rgb_t a);

	rgba_t &operator+=(float a);
	rgba_t &operator-=(float a);
	rgba_t &operator*=(float a);
	rgba_t &operator/=(float a);

	float operator[](int i);
};

rgba_t operator+(rgba_t a, rgba_t b);
rgba_t operator-(rgba_t a, rgba_t b);
rgba_t operator*(rgba_t a, rgba_t b);
rgba_t operator/(rgba_t a, rgba_t b);

rgba_t operator+(rgb_t a, rgba_t b);
rgba_t operator-(rgb_t a, rgba_t b);
rgba_t operator*(rgb_t a, rgba_t b);
rgba_t operator/(rgb_t a, rgba_t b);

rgba_t operator+(rgba_t a, rgb_t b);
rgba_t operator-(rgba_t a, rgb_t b);
rgba_t operator*(rgba_t a, rgb_t b);
rgba_t operator/(rgba_t a, rgb_t b);

rgba_t operator+(rgba_t a, float b);
rgba_t operator-(rgba_t a, float b);
rgba_t operator*(rgba_t a, float b);
rgba_t operator/(rgba_t a, float b);

rgba_t operator+(float a, rgba_t b);
rgba_t operator-(float a, rgba_t b);
rgba_t operator*(float a, rgba_t b);
rgba_t operator/(float a, rgba_t b);

#endif
