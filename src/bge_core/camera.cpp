#include "camera.h"
#include "canvas.h"

camerahdl::camerahdl()
{
	speed = 1.0;
	active = false;
}

camerahdl::~camerahdl()
{
	speed = 1.0;
	active = false;
}

void camerahdl::setup(double l, double r, double d, double u, double n, double f)
{
	speed = 1.0;
	frustum.initialize(l, r, d, u, n, f);
}

void camerahdl::translate(vec v, bool absolute)
{
	if (absolute)
		applyforce(speed*v, vec());
	else
	{
		vec o1 = degtorad(orientation);
		vec o2; // for parent object
		if (parent != NULL)
			o2 = degtorad(parent->orientation);

		space s;
		s.rotate_xyz(o1);
		s.rotate_zyx(o2);
		applyforce(speed*s.transform(v), vec());
	}
}

void camerahdl::rotate(vec v)
{
	orientation -= v;

	if (orientation.x >= 90.0)
		orientation.x = 90.0;
	else if (orientation.x <= -90.0)
		orientation.x = -90.0;
}

void camerahdl::scale(vec v)
{
	size *= v;
}

void camerahdl::projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frustum.leftf, frustum.rightf, frustum.downf, frustum.upf, frustum.nearf, frustum.farf);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void camerahdl::ortho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(frustum.leftf, frustum.rightf, frustum.downf, frustum.upf, frustum.nearf, frustum.farf);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void camerahdl::frame()
{
	glLoadIdentity();

	apply_scale();

	apply_orient_nxyz();
}

void camerahdl::generate_info(containerhdl *contain)
{
	valuehdl *mass_value = new valuehdl;
	mass_value->init((char*)"Mass:", &mass, double_ty);
	valuehdl *inertia_value = new valuehdl;
	inertia_value->init((char*)"Inertia:", &inertia, double_ty);
	valuehdl *radius_value = new valuehdl;
	radius_value->init((char*)"Radius:", &radius, double_ty);
	valuehdl *volume_value = new valuehdl;
	volume_value->init((char*)"Volume:", &volume, double_ty);
	valuehdl *density_value = new valuehdl;
	density_value->init((char*)"Density:", &density, double_ty);

	valuehdl *position_value = new valuehdl;
	position_value->init((char*)"Position:", &position, location_ty);
	valuehdl *velocity_value = new valuehdl;
	velocity_value->init((char*)"Velocity:", &velocity, location_ty);
	valuehdl *acceleration_value = new valuehdl;
	acceleration_value->init((char*)"Acceleration:", &acceleration, location_ty);

	valuehdl *orientation_value = new valuehdl;
	orientation_value->init((char*)"Orientation:", &orientation, vec_ty);
	valuehdl *avelocity_value = new valuehdl;
	avelocity_value->init((char*)"Angular Velocity:", &angular_velocity, vec_ty);
	valuehdl *aacelleration_value = new valuehdl;
	aacelleration_value->init((char*)"Angular Acceleration:", &angular_acceleration, vec_ty);

	valuehdl *size_value = new valuehdl;
	size_value->init((char*)"Size:", &size, vec_ty);

	contain->addobj(mass_value, value_t);
	contain->addobj(inertia_value, value_t);
	contain->addobj(radius_value, value_t);
	contain->addobj(volume_value, value_t);
	contain->addobj(density_value, value_t);
	contain->addobj(position_value, value_t);
	contain->addobj(velocity_value, value_t);
	contain->addobj(acceleration_value, value_t);
	contain->addobj(orientation_value, value_t);
	contain->addobj(avelocity_value, value_t);
	contain->addobj(aacelleration_value, value_t);
	contain->addobj(size_value, value_t);
}

void camerahdl::generate_menu(containerhdl *contain)
{
	checkboxhdl *remove_box = new checkboxhdl();
	remove_box->init((char*)"Delete This Object", &remove);
	checkboxhdl *active_box = new checkboxhdl();
	active_box->init((char*)"Make This Camera Active", &active);

	contain->addobj(remove_box, checkbox_t);
	contain->addobj(active_box, checkbox_t);
}

void camerahdl::initialize_derived()
{
	type = OBJ_CAMERA;
	radius = 0.05;
	volume = 4.0*m_pi*radius*radius/3.0;
	mass = 1.0;
	density = mass/volume;
	inertia = 2.0*mass*radius*radius/5.0;
}

void camerahdl::release_derived()
{
}

void camerahdl::prepare_derived()
{
	if (active && ((canvashdl*)super)->active_camera != this)
	{
		((canvashdl*)super)->active_camera = this;
		this->projection();
	}
	active = false;

	frustum.set_orientation(orientation);
}

void camerahdl::render_derived()
{
	if (super != NULL)
		glUseProgram(((canvashdl*)super)->default_pm);

	glPushMatrix();
	apply_pos();
	apply_orient_zyx();
	apply_scale();
	glutSolidSphere(radius, 8, 8);
	if (selected)
	{
		glColor3f(1.0, 0.0, 0.0);
		glutWireSphere(radius, 8, 8);
		glColor3f(1.0, 1.0, 1.0);
	}
	glScalef((frustum.rightf-frustum.leftf)*2.5, (frustum.upf-frustum.downf)*2.5, frustum.nearf*2.5);
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	glutWireCone(1.0, 1.0, 4, 1);
	glPopMatrix();
}
