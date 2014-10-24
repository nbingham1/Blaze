#include "renderer.h"

void renderhdl::init()
{
	display.init();
	camera.init(&display.framespersecond);
	camera.object = new physicshdl;
	camera.object->init(camera.framerate);
	camera.object->position.y = 100000.0;

	char name[256] = "phil";
	//s.init(name, &camera);

	p.init(name, &camera);
}

void renderhdl::release()
{
	delete camera.object;
	p.release();
	//s.release();
}

void renderhdl::displayf()
{
	if (display.vsync())
	{
		camera.object->update();
		camera.position = camera.object->position;
		//s.prepare();
		p.prepare();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.setproj();

		p.render();
		//s.render();

		display.render();

		glutSwapBuffers();
	}
}
