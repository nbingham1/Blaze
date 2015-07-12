#include "camera.h"
#include "canvas.h"

camerahdl::camerahdl()
{
	link = NULL;
	projection = perspective;
	type = "camera";
}

camerahdl::~camerahdl()
{
}

void camerahdl::view(framehdl &frame, float ratio, float front, float back)
{
	frame.projection = frustum(-ratio, ratio, -1.0f, 1.0f, front, back);

	frame.modelview = identity<float, 4, 4>();
	if (link != NULL)
		frame.modelview *= rotate_xyz(-link->orientation);
	frame.modelview *= rotate_xyz(-orientation);

	frame.modelview *= translate(-(vec3f)position);
}

void camerahdl::prepare()
{
}

void camerahdl::render(framehdl &frame)
{
	objecthdl::render(frame);

	frame.modelview.push();
	frame.modelview *= translate((vec3f)position);
	frame.modelview *= rotate_xyz(orientation);

	glutSolidSphere(1.0, 8, 8);
	if (selected)
	{
		glColor3f(1.0, 0.0, 0.0);
		glutWireSphere(1.0, 8, 8);
		glColor3f(1.0, 1.0, 1.0);
	}

	frame.modelview *= translate(vec3f(0.0, 0.0, -1.0));
	frame.modelview *= rotate_z((float)m_pi/4.0f);
	glutWireCone(1.0, 1.0, 4, 1);
	frame.modelview.pop();
}
