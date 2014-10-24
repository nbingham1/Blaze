#include "star.h"
#include "canvas.h"
#include "../common.h"
#include "../bge_mtrl.h"
#include "../bge_math.h"

starhdl::starhdl()
{
	seed = rand();
}

starhdl::starhdl(unsigned int s)
{
	seed = s;
}

starhdl::~starhdl()
{
}

void starhdl::initialize_derived()
{
	type = OBJ_STAR;

	srand(seed);

	radius = double(rand()%(100000000 - 10000000) + 10000000);
	mass = 1408.0*4*3.14159*radius*radius*radius/3;
}

void starhdl::release_derived()
{
}

void starhdl::prepare_derived()
{
}

void starhdl::render_derived()
{
	glPushMatrix();
	glUseProgram(((canvashdl*)super)->default_pm);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	vec p = position.tovec();
	if (((canvashdl*)super)->active_camera != NULL)
		p = (((canvashdl*)super)->active_camera->position - position).tovec();
	glTranslatef(-p.x, -p.y, -p.z);
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();
}

void starhdl::generate_menu(containerhdl *contain)
{
	checkboxhdl *remove_box = new checkboxhdl();
	remove_box->init((char*)"Delete This Object", &remove);

	contain->addobj(remove_box, checkbox_t);
}
