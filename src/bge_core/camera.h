#include "../common.h"
#include "../bge_math.h"
#include "object.h"

#ifndef camera_h
#define camera_h

#define OBJ_CAMERA 1

struct camerahdl : objecthdl
{
private:
	virtual void initialize_derived();
	virtual void release_derived();

	virtual void prepare_derived();
	virtual void render_derived();

public:
	camerahdl();
	~camerahdl();

	frustumhdl frustum;

	double speed;

	bool active;

	void setup(double l, double r, double d, double u, double n, double f);

	void translate(vec v, bool absolute);
	void rotate(vec v);
	void scale(vec v);

	void projection();
	void ortho();
	void frame();

		// Debug GUI
	virtual void generate_info(containerhdl *contain);
	virtual void generate_menu(containerhdl *contain);
};

#endif
