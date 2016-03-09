#include "core/geometry.h"
#include "object.h"
#include "primitive.h"

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

	void render(framehdl &frame);
};

#endif
