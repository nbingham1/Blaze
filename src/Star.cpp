#include "star.h"

void star::init(char *n, camerahdl *cam)
{
	camera.ref = cam;
	first = NULL;
	last = NULL;
	strcpy(name, n);
	for (unsigned int x = 0; x < strlen(n); x++)
		seed += int(n[x]);

	srand(seed);

	physics.init(cam->framerate);
	physics.radius = float(rand()%90000000 + 10000000);

	material.init("res/star.vx", "res/star.ft");
	unsigned char *texdata = new unsigned char[32*32*32*4];

	noise3tex(texdata, 32);
	material.addtex(texdata, 32, 32, 32, true, GL_RGBA, GL_REPEAT);
	delete texdata;

	camera.ground_height = -physics.radius;

	num_planets = rand()%10;
	char name[256];
	for (int x = 0; x < num_planets; x++)
	{
		for (int y = 0; y < 4; y++)
			name[y] = rand()%128 + 128;
		name[5] = '\0';

		addplanet(name);
	}
}

void star::release()
{
	material.release();

	planet *curr = first, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		prev->release();
		delete prev;
	}
}

void star::addplanet(char *name)
{
	planet *p = new planet;
	p->init(name, camera.ref);

	if (first == NULL)
	{
		first = p;
		last = p;

		p->physics.position.x = 100000000.0;
	}
	else
	{
		p->physics.position.x = 2*last->physics.position.x;
		last->next = p;
		last->next->prev = last;
		last = last->next;
	}
}

void star::prepare()
{
	physics.update();
	camera.update(&physics);

	planet *curr = first;
	while (curr != NULL)
	{
		//curr->physics.forces +=
		curr->prepare();
		curr = curr->next;
	}
}

void star::render()
{
	double dist = 1E99, max = 0.0;
	planet *curr, *render;
	for (int x = 0; x < num_planets+1; x++)
	{
		curr = first;
		while (curr != NULL)
		{
			if (curr->camera.distance > max && curr->camera.distance < dist)
			{
				max = curr->camera.distance;
				render = curr;
			}
			curr = curr->next;
		}
		if (camera.distance > max && camera.distance < dist)
		{
			dist = camera.distance;
			max = 0.0;
			vec pos = camera.vector*camera.distance;
			material.bind();
			glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			glutSolidSphere(physics.radius, 20, 20);
			glPopMatrix();
		}
		else
		{
			dist = render->camera.distance;
			max = 0.0;
			render->render();
		}
		if (x < num_planets)
			glClear(GL_DEPTH_BUFFER_BIT);
	}
}
