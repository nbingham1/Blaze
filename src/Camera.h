#include "vector.h"
#include "physics.h"

#ifndef camera_h
#define camera_h

struct camerahdl;
struct cameraref;

struct camerahdl
{
	vec position;
	vec orientation;
	vec lookat;
	double scale;
	double oldscale;

	double speed;
	unsigned int person;

	double *framerate;

	physicshdl *object;

	float *texcoord;
	float *sphereverts;
	unsigned int *indices;
	int size;
	int xsize;

	void init(double *rateptr);
	void release();

	void move(vec v);
	void forward(double mag);
	void backward(double mag);
	void right(double mag);
	void left(double mag);
	void up(double mag);
	void down(double mag);

	void rotate(vec v);

	void setproj();
};

struct cameraref
{
	camerahdl *ref;
	vec    vector,   nvector;
	vec    orient;
	double distance, ndistance;
	double visangle, nvisangle;
	double radius;
	double ground_height;

	GLfloat spheremat[16];

	void update(physicshdl *obj);
	void render();
};

#endif
