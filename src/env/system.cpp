#include "system.h"
#include "galaxy.h"

void systemhdl::init(galaxyhdl *o)
{
	star_list = NULL;
	num_stars = 0;

	planet_list = NULL;
	num_planets = 0;

	asteroid_list = NULL;
	num_asteroids = 0;

	orgin = o;
}

bool systemhdl::generate()
{
	return true;
}

void systemhdl::release()
{
	if (star_list != NULL)
	{
		for (int x = 0; x < num_stars; x++)
			star_list[x].release();

		delete [] star_list;
		star_list = NULL;
	}

	num_stars = 0;

	if (planet_list != NULL)
	{
		for (int x = 0; x < num_planets; x++)
			planet_list[x].release();

		delete [] planet_list;
		planet_list = NULL;
	}

	num_planets = 0;

	if (asteroid_list != NULL)
	{
		for (int x = 0; x < num_asteroids; x++)
			asteroid_list[x].release();
		delete [] asteroid_list;
		asteroid_list = NULL;
	}

	num_asteroids = 0;
}

void systemhdl::prepare()
{
	for (int x = 0; x < num_stars; x++)
		star_list[x].prepare();

	for (int x = 0; x < num_planets; x++)
		planet_list[x].prepare();

	for (int x = 0; x < num_asteroids; x++)
		asteroid_list[x].prepare();
}

void systemhdl::render()
{
	for (int x = 0; x < num_stars; x++)
		star_list[x].render();

	for (int x = 0; x < num_planets; x++)
		planet_list[x].render();

	for (int x = 0; x < num_asteroids; x++)
		asteroid_list[x].render();
}

vec systemhdl::camerapos()
{
	return orgin->camerapos();
}
