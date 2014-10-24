#include "renderer.h"

void renderhdl::init()
{
	display.init();
	camera.init(&display.framespersecond);
	camera.object = new physicshdl;
	camera.object->init(camera.framerate);

	universe.init(7, &camera);
	universe.generate(80000);
	//galaxy.init(&universe);
	///galaxy.generate(100000, 1);
}

void renderhdl::release()
{
	delete camera.object;

	galaxy.release();
	//universe.release();
}

void renderhdl::displayf()
{
	glDisable(GL_LIGHTING);
	if (display.vsync())
	{
		camera.object->update();
		camera.position = camera.object->position;

		universe.prepare();
		//galaxy.prepare();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.setproj();

		universe.render();
		//galaxy.render();

		display.render();

		glutSwapBuffers();
	}
}
