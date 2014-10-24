#ifndef color_h
#define color_h

struct rgb_hdr; // red, green, blue: high dynamic range
struct rgba_hdr; // red, green, blue, alpha: high dynamic range
struct rgb_ldr; // red, green, blue: low dynamic range
struct rgba_ldr; // red, green, blue, alpha: low dynamic range

struct rgb_hdr
{
	rgb_hdr();
	rgb_hdr(float i); // greyscale intensity
	rgb_hdr(float r, float g, float b); // red, blue, green
	~rgb_hdr();

	float r;
	float g;
	float b;

	rgb_hdr &operator=(rgb_hdr a);
	rgb_hdr &operator=(rgba_hdr a);
	rgb_hdr &operator=(rgb_ldr a);
	rgb_hdr &operator=(rgba_ldr a);

	rgb_hdr &operator+=(rgb_hdr a);
	rgb_hdr &operator-=(rgb_hdr a);
	rgb_hdr &operator*=(rgb_hdr a);
	rgb_hdr &operator/=(rgb_hdr a);

	rgb_hdr &operator+=(rgba_hdr a);
	rgb_hdr &operator-=(rgba_hdr a);
	rgb_hdr &operator*=(rgba_hdr a);
	rgb_hdr &operator/=(rgba_hdr a);

	rgb_hdr &operator+=(rgb_ldr a);
	rgb_hdr &operator-=(rgb_ldr a);
	rgb_hdr &operator*=(rgb_ldr a);
	rgb_hdr &operator/=(rgb_ldr a);

	rgb_hdr &operator+=(rgba_ldr a);
	rgb_hdr &operator-=(rgba_ldr a);
	rgb_hdr &operator*=(rgba_ldr a);
	rgb_hdr &operator/=(rgba_ldr a);

	rgb_hdr &operator+=(float a);
	rgb_hdr &operator-=(float a);
	rgb_hdr &operator*=(float a);
	rgb_hdr &operator/=(float a);

	rgb_hdr &operator+=(unsigned char a);
	rgb_hdr &operator-=(unsigned char a);
	rgb_hdr &operator*=(unsigned char a);
	rgb_hdr &operator/=(unsigned char a);

	float operator[](int i);
};

rgb_hdr operator+(rgb_hdr a, rgb_hdr b);
rgb_hdr operator-(rgb_hdr a, rgb_hdr b);
rgb_hdr operator*(rgb_hdr a, rgb_hdr b);
rgb_hdr operator/(rgb_hdr a, rgb_hdr b);

rgb_hdr operator+(rgb_hdr a, rgba_hdr b);
rgb_hdr operator-(rgb_hdr a, rgba_hdr b);
rgb_hdr operator*(rgb_hdr a, rgba_hdr b);
rgb_hdr operator/(rgb_hdr a, rgba_hdr b);

rgb_hdr operator+(rgb_hdr a, rgb_ldr b);
rgb_hdr operator-(rgb_hdr a, rgb_ldr b);
rgb_hdr operator*(rgb_hdr a, rgb_ldr b);
rgb_hdr operator/(rgb_hdr a, rgb_ldr b);

rgb_hdr operator+(rgb_hdr a, rgba_ldr b);
rgb_hdr operator-(rgb_hdr a, rgba_ldr b);
rgb_hdr operator*(rgb_hdr a, rgba_ldr b);
rgb_hdr operator/(rgb_hdr a, rgba_ldr b);

rgb_hdr operator+(rgb_hdr a, float b);
rgb_hdr operator-(rgb_hdr a, float b);
rgb_hdr operator*(rgb_hdr a, float b);
rgb_hdr operator/(rgb_hdr a, float b);

rgb_hdr operator+(float a, rgb_hdr b);
rgb_hdr operator-(float a, rgb_hdr b);
rgb_hdr operator*(float a, rgb_hdr b);
rgb_hdr operator/(float a, rgb_hdr b);

rgb_hdr operator+(rgb_hdr a, unsigned char b);
rgb_hdr operator-(rgb_hdr a, unsigned char b);
rgb_hdr operator*(rgb_hdr a, unsigned char b);
rgb_hdr operator/(rgb_hdr a, unsigned char b);

rgb_hdr operator+(unsigned char a, rgb_hdr b);
rgb_hdr operator-(unsigned char a, rgb_hdr b);
rgb_hdr operator*(unsigned char a, rgb_hdr b);
rgb_hdr operator/(unsigned char a, rgb_hdr b);

struct rgba_hdr
{
	rgba_hdr();
	rgba_hdr(float i); // greyscale intensity
	rgba_hdr(float i, float a); // greyscale instensity, alpha
	rgba_hdr(float r, float g, float b); // red, blue, green
	rgba_hdr(float r, float g, float b, float a); // red, blue, green, alpha
	~rgba_hdr();

	float r;
	float g;
	float b;
	float a;

	rgba_hdr &operator=(rgba_hdr a);
	rgba_hdr &operator=(rgb_hdr a);
	rgba_hdr &operator=(rgba_ldr a);
	rgba_hdr &operator=(rgb_ldr a);

	rgba_hdr &operator+=(rgba_hdr a);
	rgba_hdr &operator-=(rgba_hdr a);
	rgba_hdr &operator*=(rgba_hdr a);
	rgba_hdr &operator/=(rgba_hdr a);

	rgba_hdr &operator+=(rgb_hdr a);
	rgba_hdr &operator-=(rgb_hdr a);
	rgba_hdr &operator*=(rgb_hdr a);
	rgba_hdr &operator/=(rgb_hdr a);

	rgba_hdr &operator+=(rgba_ldr a);
	rgba_hdr &operator-=(rgba_ldr a);
	rgba_hdr &operator*=(rgba_ldr a);
	rgba_hdr &operator/=(rgba_ldr a);

	rgba_hdr &operator+=(rgb_ldr a);
	rgba_hdr &operator-=(rgb_ldr a);
	rgba_hdr &operator*=(rgb_ldr a);
	rgba_hdr &operator/=(rgb_ldr a);

	rgba_hdr &operator+=(float a);
	rgba_hdr &operator-=(float a);
	rgba_hdr &operator*=(float a);
	rgba_hdr &operator/=(float a);

	rgba_hdr &operator+=(unsigned char a);
	rgba_hdr &operator-=(unsigned char a);
	rgba_hdr &operator*=(unsigned char a);
	rgba_hdr &operator/=(unsigned char a);

	float operator[](int i);
};

rgba_hdr operator+(rgba_hdr a, rgba_hdr b);
rgba_hdr operator-(rgba_hdr a, rgba_hdr b);
rgba_hdr operator*(rgba_hdr a, rgba_hdr b);
rgba_hdr operator/(rgba_hdr a, rgba_hdr b);

rgba_hdr operator+(rgba_hdr a, rgb_hdr b);
rgba_hdr operator-(rgba_hdr a, rgb_hdr b);
rgba_hdr operator*(rgba_hdr a, rgb_hdr b);
rgba_hdr operator/(rgba_hdr a, rgb_hdr b);

rgba_hdr operator+(rgba_hdr a, rgba_ldr b);
rgba_hdr operator-(rgba_hdr a, rgba_ldr b);
rgba_hdr operator*(rgba_hdr a, rgba_ldr b);
rgba_hdr operator/(rgba_hdr a, rgba_ldr b);

rgba_hdr operator+(rgba_hdr a, rgb_ldr b);
rgba_hdr operator-(rgba_hdr a, rgb_ldr b);
rgba_hdr operator*(rgba_hdr a, rgb_ldr b);
rgba_hdr operator/(rgba_hdr a, rgb_ldr b);

rgba_hdr operator+(rgba_hdr a, float b);
rgba_hdr operator-(rgba_hdr a, float b);
rgba_hdr operator*(rgba_hdr a, float b);
rgba_hdr operator/(rgba_hdr a, float b);

rgba_hdr operator+(float a, rgba_hdr b);
rgba_hdr operator-(float a, rgba_hdr b);
rgba_hdr operator*(float a, rgba_hdr b);
rgba_hdr operator/(float a, rgba_hdr b);

rgba_hdr operator+(rgba_hdr a, unsigned char b);
rgba_hdr operator-(rgba_hdr a, unsigned char b);
rgba_hdr operator*(rgba_hdr a, unsigned char b);
rgba_hdr operator/(rgba_hdr a, unsigned char b);

rgba_hdr operator+(unsigned char a, rgba_hdr b);
rgba_hdr operator-(unsigned char a, rgba_hdr b);
rgba_hdr operator*(unsigned char a, rgba_hdr b);
rgba_hdr operator/(unsigned char a, rgba_hdr b);

struct rgb_ldr
{
	rgb_ldr();
	rgb_ldr(unsigned char i); // greyscale intensity
	rgb_ldr(unsigned char r, unsigned char g, unsigned char b); // red, blue, green
	~rgb_ldr();

	unsigned char r;
	unsigned char g;
	unsigned char b;

	rgb_ldr &operator=(rgb_ldr a);
	rgb_ldr &operator=(rgba_ldr a);
	rgb_ldr &operator=(rgb_hdr a);
	rgb_ldr &operator=(rgba_hdr a);

	rgb_ldr &operator+=(rgb_ldr a);
	rgb_ldr &operator-=(rgb_ldr a);
	rgb_ldr &operator*=(rgb_ldr a);
	rgb_ldr &operator/=(rgb_ldr a);

	rgb_ldr &operator+=(rgba_ldr a);
	rgb_ldr &operator-=(rgba_ldr a);
	rgb_ldr &operator*=(rgba_ldr a);
	rgb_ldr &operator/=(rgba_ldr a);

	rgb_ldr &operator+=(rgb_hdr a);
	rgb_ldr &operator-=(rgb_hdr a);
	rgb_ldr &operator*=(rgb_hdr a);
	rgb_ldr &operator/=(rgb_hdr a);

	rgb_ldr &operator+=(rgba_hdr a);
	rgb_ldr &operator-=(rgba_hdr a);
	rgb_ldr &operator*=(rgba_hdr a);
	rgb_ldr &operator/=(rgba_hdr a);

	rgb_ldr &operator+=(unsigned char a);
	rgb_ldr &operator-=(unsigned char a);
	rgb_ldr &operator*=(unsigned char a);
	rgb_ldr &operator/=(unsigned char a);

	rgb_ldr &operator+=(float a);
	rgb_ldr &operator-=(float a);
	rgb_ldr &operator*=(float a);
	rgb_ldr &operator/=(float a);

	unsigned char operator[](int i);
};

rgb_ldr operator+(rgb_ldr a, rgb_ldr b);
rgb_ldr operator-(rgb_ldr a, rgb_ldr b);
rgb_ldr operator*(rgb_ldr a, rgb_ldr b);
rgb_ldr operator/(rgb_ldr a, rgb_ldr b);

rgb_ldr operator+(rgb_ldr a, rgba_ldr b);
rgb_ldr operator-(rgb_ldr a, rgba_ldr b);
rgb_ldr operator*(rgb_ldr a, rgba_ldr b);
rgb_ldr operator/(rgb_ldr a, rgba_ldr b);

rgb_ldr operator+(rgb_ldr a, rgb_hdr b);
rgb_ldr operator-(rgb_ldr a, rgb_hdr b);
rgb_ldr operator*(rgb_ldr a, rgb_hdr b);
rgb_ldr operator/(rgb_ldr a, rgb_hdr b);

rgb_ldr operator+(rgb_ldr a, rgba_hdr b);
rgb_ldr operator-(rgb_ldr a, rgba_hdr b);
rgb_ldr operator*(rgb_ldr a, rgba_hdr b);
rgb_ldr operator/(rgb_ldr a, rgba_hdr b);

rgb_ldr operator+(rgb_ldr a, unsigned char b);
rgb_ldr operator-(rgb_ldr a, unsigned char b);
rgb_ldr operator*(rgb_ldr a, unsigned char b);
rgb_ldr operator/(rgb_ldr a, unsigned char b);

rgb_ldr operator+(unsigned char a, rgb_ldr b);
rgb_ldr operator-(unsigned char a, rgb_ldr b);
rgb_ldr operator*(unsigned char a, rgb_ldr b);
rgb_ldr operator/(unsigned char a, rgb_ldr b);

rgb_ldr operator+(rgb_ldr a, float b);
rgb_ldr operator-(rgb_ldr a, float b);
rgb_ldr operator*(rgb_ldr a, float b);
rgb_ldr operator/(rgb_ldr a, float b);

rgb_ldr operator+(float a, rgb_ldr b);
rgb_ldr operator-(float a, rgb_ldr b);
rgb_ldr operator*(float a, rgb_ldr b);
rgb_ldr operator/(float a, rgb_ldr b);

struct rgba_ldr
{
	rgba_ldr();
	rgba_ldr(unsigned char i); // greyscale intensity
	rgba_ldr(unsigned char i, unsigned char a); // greyscale instensity, alpha
	rgba_ldr(unsigned char r, unsigned char g, unsigned char b); // red, blue, green
	rgba_ldr(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // red, blue, green, alpha
	~rgba_ldr();

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	rgba_ldr &operator=(rgba_ldr a);
	rgba_ldr &operator=(rgb_ldr a);
	rgba_ldr &operator=(rgba_hdr a);
	rgba_ldr &operator=(rgb_hdr a);

	rgba_ldr &operator+=(rgba_ldr a);
	rgba_ldr &operator-=(rgba_ldr a);
	rgba_ldr &operator*=(rgba_ldr a);
	rgba_ldr &operator/=(rgba_ldr a);

	rgba_ldr &operator+=(rgb_ldr a);
	rgba_ldr &operator-=(rgb_ldr a);
	rgba_ldr &operator*=(rgb_ldr a);
	rgba_ldr &operator/=(rgb_ldr a);

	rgba_ldr &operator+=(rgba_hdr a);
	rgba_ldr &operator-=(rgba_hdr a);
	rgba_ldr &operator*=(rgba_hdr a);
	rgba_ldr &operator/=(rgba_hdr a);

	rgba_ldr &operator+=(rgb_hdr a);
	rgba_ldr &operator-=(rgb_hdr a);
	rgba_ldr &operator*=(rgb_hdr a);
	rgba_ldr &operator/=(rgb_hdr a);

	rgba_ldr &operator+=(unsigned char a);
	rgba_ldr &operator-=(unsigned char a);
	rgba_ldr &operator*=(unsigned char a);
	rgba_ldr &operator/=(unsigned char a);

	rgba_ldr &operator+=(float a);
	rgba_ldr &operator-=(float a);
	rgba_ldr &operator*=(float a);
	rgba_ldr &operator/=(float a);

	unsigned char operator[](int i);
};

rgba_ldr operator+(rgba_ldr a, rgba_ldr b);
rgba_ldr operator-(rgba_ldr a, rgba_ldr b);
rgba_ldr operator*(rgba_ldr a, rgba_ldr b);
rgba_ldr operator/(rgba_ldr a, rgba_ldr b);

rgba_ldr operator+(rgba_ldr a, rgb_ldr b);
rgba_ldr operator-(rgba_ldr a, rgb_ldr b);
rgba_ldr operator*(rgba_ldr a, rgb_ldr b);
rgba_ldr operator/(rgba_ldr a, rgb_ldr b);

rgba_ldr operator+(rgba_ldr a, rgba_hdr b);
rgba_ldr operator-(rgba_ldr a, rgba_hdr b);
rgba_ldr operator*(rgba_ldr a, rgba_hdr b);
rgba_ldr operator/(rgba_ldr a, rgba_hdr b);

rgba_ldr operator+(rgba_ldr a, rgb_hdr b);
rgba_ldr operator-(rgba_ldr a, rgb_hdr b);
rgba_ldr operator*(rgba_ldr a, rgb_hdr b);
rgba_ldr operator/(rgba_ldr a, rgb_hdr b);

rgba_ldr operator+(rgba_ldr a, unsigned char b);
rgba_ldr operator-(rgba_ldr a, unsigned char b);
rgba_ldr operator*(rgba_ldr a, unsigned char b);
rgba_ldr operator/(rgba_ldr a, unsigned char b);

rgba_ldr operator+(unsigned char a, rgba_ldr b);
rgba_ldr operator-(unsigned char a, rgba_ldr b);
rgba_ldr operator*(unsigned char a, rgba_ldr b);
rgba_ldr operator/(unsigned char a, rgba_ldr b);

rgba_ldr operator+(rgba_ldr a, float b);
rgba_ldr operator-(rgba_ldr a, float b);
rgba_ldr operator*(rgba_ldr a, float b);
rgba_ldr operator/(rgba_ldr a, float b);

rgba_ldr operator+(float a, rgba_ldr b);
rgba_ldr operator-(float a, rgba_ldr b);
rgba_ldr operator*(float a, rgba_ldr b);
rgba_ldr operator/(float a, rgba_ldr b);

#endif
