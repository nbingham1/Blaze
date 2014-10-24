#include "galaxy.h"
#include "universe.h"

void galaxyhdl::init(universehdl *o, int s)
{
	system = NULL;
	system_list = NULL;
	num_systems = 0;

	nebula = NULL;
	nebula_list = NULL;
	num_nebula = 0;

	ostree = NULL;

	orgin = o;
	seed = s;

	next = NULL;
	prev = NULL;
}

void *galaxyhdl::generate(void *buf)
{
	int ns, nn;

	ns = ((int*)buf)[0];
	nn = ((int*)buf)[1];

	srand(seed);
	num_systems = ns;
	system_list = NULL;
	system_list = new double[ns*3];
	if (system_list == NULL)
	{
		printf("{Error: Not enough memory, cleaning up}\n");
		release();
		return false;
	}

	double t, c, s, d;

	double ellipse = 1.0;//double(rand()%2);
	c = double(rand()%100)/50.0;

	double xp, yp;

	ostree = new octnode_i;
	ostree->init(system_list, 1e5, -1e5, 1e5, -1e5, 1e5, -1e5);

	for (int x = 0; x < num_systems; x++)
	{
		t = (double(rand()%100)/100.0)*double(rand()%360)*m_pi/180.0;
		s = rand()%2;
		s = (s == 0.0 ? -1.0 : s);

		xp = double(rand()%16000)*10000000/16000 - 5000000;
		yp = double(rand()%16000)*10000000/16000 - 5000000;
		d = sqrt(xp*xp + yp*yp)/80000.0;
		xp /= d;
		yp /= d;

		system_list[x*3 + 0] = s*1e5*(t*ellipse)*cos(t*c) + xp*(2.0*m_pi - t)/m_pi;
		system_list[x*3 + 2] = s*1e5*(t*ellipse)*sin(t*c) + yp*(2.0*m_pi - t)/m_pi;

		d = (system_list[x*3 + 0]*system_list[x*3 + 0] + system_list[x*3 + 2]*system_list[x*3 + 2])/1e10;

		system_list[x*3 + 1] = (double(rand()%16000)*80000/16000 - 40000)*(atan(-d/1.2 + 1.5) + 1.5);

		ostree->classify(x, &system_list[x*3]);
	}

	num_nebula = nn;
	nebula_list = NULL;
	nebula_list = new double[nn*3];
	if (nebula_list == NULL)
	{
		printf("{Error: Not enough memory, cleaning up}\n");
		release();
		return (void*)false;
	}

	for (int x = 0; x < num_systems; x++)
	{
		//nebula_list[x*3 + 0]
	}

	return (void*)true;
}

void galaxyhdl::release()
{
	if (system_list != NULL)
	{
		delete [] system_list;
		system_list = NULL;
	}
	if (system != NULL)
	{
		system->release();
		delete system;
		system = NULL;
	}

	num_systems = 0;

	if (nebula_list != NULL)
	{
		delete [] nebula_list;
		nebula_list = NULL;
	}
	if (nebula != NULL)
	{
		nebula->release();
		delete nebula;
		nebula = NULL;
	}

	num_nebula = 0;

	if (ostree != NULL)
	{
		ostree->release();
		delete ostree;
		ostree = NULL;
	}
	next = NULL;
	prev = NULL;
}

void galaxyhdl::prepare()
{
	/*vec cam = campos();
	if (system != NULL)
	{
		system->prepare();
		if (dot(physics.position + system->physics.position + camera->position, physics.position + system->physics.position + camera->position) > 100)
		{
			system->release();
			delete system;
			system = NULL;
		}
	}
	else
	{
		int num = -1;
		double arr[] = {-camera->position.x, -camera->position.y, -camera->position.z};
		int *ptr = ostree->getp(arr, &num);

		for (int x = 0; x < num; x++)
		{
			if (dot(vec(&galaxy_list[ptr[x]*3], 3) + camera->position, vec(&galaxy_list[ptr[x]*3], 3) + camera->position) < 25e14)
			{
				galaxy = new galaxyhdl;
				srand(seed);
				galaxy->init(this, (rand()%num_galaxies)*ptr[x]);
				galaxy->generate(10000, 0);
				galaxy->physics.position = vec(&galaxy_list[ptr[x]*3], 3);
			}
		}
	}

	if (nebula != NULL)
		nebula->prepare();*/
}

void galaxyhdl::render()
{
	double v[num_systems*3];
	double c[num_systems*4];

	vec cam = camerapos();
	double dist;

	for (int x = 0; x < num_systems; x++)
	{
		v[x*3 + 0] = system_list[x*3 + 0] + cam.x + physics.position.x;
		v[x*3 + 1] = system_list[x*3 + 1] + cam.y + physics.position.y;
		v[x*3 + 2] = system_list[x*3 + 2] + cam.z + physics.position.z;

		dist = (system_list[x*3 + 0]*system_list[x*3 + 0] +
			   system_list[x*3 + 1]*system_list[x*3 + 1] +
			   system_list[x*3 + 2]*system_list[x*3 + 2])/9.0;

		c[x*4 + 0] = (1e10 - dist)/1e10;
		c[x*4 + 1] = (1e10 - dist)/1e10;
		c[x*4 + 2] = dist/1e10;
		c[x*4 + 3] = 1.0;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, v);
	glColorPointer(4, GL_DOUBLE, 0, c);

	glDrawArrays(GL_POINTS, 0, num_systems);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (system != NULL)
		system->render();
	if (nebula != NULL)
		nebula->render(false);
}

vec galaxyhdl::camerapos()
{
	return orgin->camerapos();
}
