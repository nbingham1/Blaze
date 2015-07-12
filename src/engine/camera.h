#include "core/geometry.h"
#include "object.h"

using namespace core;

#ifndef camera_h
#define camera_h

struct camerahdl : objecthdl
{
	camerahdl();
	~camerahdl();

	enum
	{
		perspective,
		orthographic
	} projection;

	objecthdl *link;

	void view(framehdl &frame, float ratio, float front, float back);

	void prepare();
	void render(framehdl &frame);
};

#endif
