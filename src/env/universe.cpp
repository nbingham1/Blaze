#include "universe.h"

void universehdl::init(int s, camerahdl *cam)
{
	num_galaxies = 0;
	galaxy_list = NULL;
	galaxy_s = NULL;
	galaxy_e = NULL;
	ostree = NULL;

	seed = s;

	camera = cam;
	for (int  x = 0; x < 3; x++)
		cam_address[x] = -1;
}

bool universehdl::generate(int n)
{
	printf("[Generating Universe]\n");
	num_galaxies = n;
	galaxy_list = NULL;
	galaxy_list = new double[n*3];
	if (galaxy_list == NULL)
	{
		printf("{Error: Not enough memory, cleaning up}\n");
		release();
		return false;
	}

	ostree = new octnode_i;
	ostree->init(galaxy_list, 4.1e9, -4.1e9, 4.1e9, -4.1e9, 4.1e9, -4.1e9);

	for (int x = 0; x < num_galaxies; x++)
	{
		galaxy_list[x*3 + 0] = double(rand()%8116)*1e6 - 4.1e9;
		galaxy_list[x*3 + 1] = double(rand()%8116)*1e6 - 4.1e9;
		galaxy_list[x*3 + 2] = double(rand()%8116)*1e6 - 4.1e9;
		ostree->classify(x, &galaxy_list[x*3]);
	}

	return true;
}

void universehdl::addgalaxy(galaxyhdl *g)
{
	if (galaxy_s == NULL || galaxy_e == NULL)
	{
		galaxy_s = g;
		galaxy_e = g;
	}
	else
	{
		galaxy_e->next = g;
		g->prev = galaxy_e;
		galaxy_e = galaxy_e->next;
	}
}

void universehdl::remgalaxy(galaxyhdl *g)
{
	if (galaxy_s == g)
		galaxy_s = galaxy_s->next;
	if (galaxy_e == g)
		galaxy_e = galaxy_e->prev;

	if (g->next != NULL)
		g->next->prev = g->prev;
	if (g->prev != NULL)
		g->prev->next = g->next;

	g->release();
	delete g;
	g = NULL;
}

void universehdl::release()
{
	if (galaxy_list != NULL)
	{
		delete [] galaxy_list;
		galaxy_list = NULL;
	}

	galaxyhdl *curr = galaxy_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		prev->release();
		delete prev;
		prev = NULL;
	}

	num_galaxies = 0;

	if (ostree != NULL)
	{
		ostree->release();
		delete ostree;
		ostree = NULL;
	}
}

void universehdl::prepare()
{
	galaxyhdl *curr = galaxy_s, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		prev->prepare();
		if (dot(prev->physics.position + camera->position, prev->physics.position + camera->position) > (double)25E12)
		{
			if (prev->next != NULL)
				prev->next->prev = prev->prev;
			if (prev->prev != NULL)
				prev->prev->next = prev->next;

			if (prev == galaxy_e)
				galaxy_e = prev->prev;
			if (prev == galaxy_s)
				galaxy_s = prev->next;

			prev->release();
			delete prev;
			prev = NULL;
		}
	}

	int num = -1;
	int iarr[] = {2000, 0};
	double arr[] = {-camera->position.x, -camera->position.y, -camera->position.z};
	int *ptr = ostree->getp(arr, &num);
	galaxyhdl *galaxy;

	for (int x = 0; x < num; x++)
	{
		if (dot(vec(&galaxy_list[ptr[x]*3], 3) + camera->position, vec(&galaxy_list[ptr[x]*3], 3) + camera->position) < (double)25E12)
		{
			galaxy = new galaxyhdl;
			srand(seed);
			galaxy->init(this, (rand()%num_galaxies)*ptr[x]);
			galaxy->generate(iarr);
			galaxy->physics.position = vec(&galaxy_list[ptr[x]*3], 3);
			addgalaxy(galaxy);
		}
	}
}

void universehdl::render()
{
	double *v = new double[num_galaxies*3];
	double *c = new double[num_galaxies*4];

	vec cam = camerapos();

	for (int x = 0; x < num_galaxies; x++)
	{
		v[x*3 + 0] = galaxy_list[x*3 + 0] + cam.x;
		v[x*3 + 1] = galaxy_list[x*3 + 1] + cam.y;
		v[x*3 + 2] = galaxy_list[x*3 + 2] + cam.z;

		c[x*4 + 0] = 1.0;
		c[x*4 + 1] = 1.0;
		c[x*4 + 2] = 1.0;
		c[x*4 + 3] = (16e18 - (v[x*3 + 0]*v[x*3 + 0] + v[x*3 + 1]*v[x*3 + 1] + v[x*3 + 2]*v[x*3 + 2]))/16e18;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, v);
	glColorPointer(4, GL_DOUBLE, 0, c);

	glDrawArrays(GL_POINTS, 0, num_galaxies);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	galaxyhdl *curr = galaxy_s;
	while (curr != NULL)
	{
		curr->render();
		curr = curr->next;
	}

	delete [] v;
	delete [] c;
}

vec universehdl::camerapos()
{
	return camera->position;
}
