#include "camera.h"
#include "standard.h"
#include "graphics.h"
#include "mathematics.h"

void camerahdl::init(double *rateptr)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.6/10.0, 1.6/10.0, -1.0/10.0, 1.0/10.0, .2, 999999999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	scale = 1.0;
	person = 1;
	speed = 1.0;

	framerate = rateptr;

	object = NULL;

	size = 64; // must be power of two
	xsize = 64;
	sphereverts = new float[3*xsize*size];
	texcoord = new float[xsize*size];
	indices = new unsigned int[xsize*size*6];
	float x_temp, y_temp;
	vec v;
	for (int x = 0; x < xsize; x++)
	{
		x_temp = 2.0*m_pi*float(xsize-1 - x)/float(xsize-1);
		for (int y = 0; y < size; y++)
		{
			y_temp = m_pi*float(y)/float(size-1) - m_pi/2.0;

			v.x = cos(x_temp) * cos(y_temp);
			v.y = sin(y_temp);
			v.z = sin(x_temp) * cos(y_temp);

			sphereverts[3*(x*size + y) + 0] = v.x;
			sphereverts[3*(x*size + y) + 1] = v.y;
			sphereverts[3*(x*size + y) + 2] = v.z;

			texcoord[x*size + y] = float(y)/float(size-1);
		}
	}

	int index = 0;
	for (int x = 1; x < xsize; x++)
	{
		for (int y = 0; y < size; y++)
		{
			indices[index++] = x*size+y;
			indices[index++] = (x-1)*size+y;
		}
	}
}

void camerahdl::release()
{
	delete [] texcoord;
	delete [] texcoord;
	delete [] indices;
}

void camerahdl::move(vec v)
{
	vec o2;

	if (object != NULL)
		o2 = degtorad(object->orientation);

	vec o1 = -degtorad(orientation);

	if (*framerate == 0.0)
		*framerate = 1.0;

	if (object != NULL)
		object->forces += (speed/(*framerate))*(v.x*rotx(roty(rotz(rotz(roty(rotx(vec(1.0, 0.0, 0.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x) +
											  v.y*rotx(roty(rotz(rotz(roty(rotx(vec(0.0, 1.0, 0.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x) +
											  v.z*rotx(roty(rotz(rotz(roty(rotx(vec(0.0, 0.0, 1.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x));
	else
		position += (speed/(*framerate))*(v.x*rotx(roty(rotz(rotz(roty(rotx(vec(1.0, 0.0, 0.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x) +
						   v.y*rotx(roty(rotz(rotz(roty(rotx(vec(0.0, 1.0, 0.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x) +
						   v.z*rotx(roty(rotz(rotz(roty(rotx(vec(0.0, 0.0, 1.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x));
}

void camerahdl::forward(double mag)
{
	move(vec(0.0, 0.0, mag));
}

void camerahdl::backward(double mag)
{
	move(vec(0.0, 0.0, -mag));
}

void camerahdl::right(double mag)
{
	move(vec(-mag, 0.0, 0.0));
}

void camerahdl::left(double mag)
{
	move(vec(mag, 0.0, 0.0));
}

void camerahdl::up(double mag)
{
	move(vec(0.0, -mag, 0.0));
}

void camerahdl::down(double mag)
{
	move(vec(0.0, mag, 0.0));
}

void camerahdl::rotate(vec v)
{
	orientation.x += v.x;
	orientation.y += v.y;
	orientation.z += v.z;
	if (orientation.x >= 90.0)
		orientation.x = 90.0;

	else if (orientation.x <= -80.0)
		orientation.x = -80.0;
}

void camerahdl::setproj()
{
	//vec o1 = -degtorad(orientation);
	//vec o2 = object != NULL ? degtorad(object->orientation) : vec();
	//lookat = rotx(roty(rotz(rotz(roty(rotx(vec(0.0, 0.0, 1.0), o1.x), o1.y), o1.z), o2.z), o2.y), o2.x);

	glLoadIdentity();
	if (scale == -1.0)
		scale = 1.0;
	glScaled(scale, scale, scale);
    glRotated(orientation.x, 1.0, 0.0, 0.0);
    glRotated(orientation.y, 0.0, 1.0, 0.0);
    glRotated(orientation.z, 0.0, 0.0, 1.0);

   /* if (attachment != NULL && attachment->geometry != NULL)
    {
    	glPushMatrix();
    	attachment->geometry->render();
    	glPopMatrix();
    }*/

	if (object != NULL)
	{
		 glRotatef(object->orientation.z, 0, 0, -1);
		 glRotatef(object->orientation.y, 0, -1, 0);
		 glRotatef(object->orientation.x, -1, 0, 0);
		 //glTranslated(-object->position.x, -object->position.y, -object->position.z);
	}
	//else
	//	glTranslatef(position.x, position.y, position.z);

	oldscale = scale;
	scale = -1.0;
}

void cameraref::update(physicshdl *obj)
{
	distance = dist(ref->position, obj->position);
	vector = (ref->position - obj->position)/distance;
	double scale;
	if (distance - obj->radius - ground_height < 7.0 || distance - obj->radius < 7.0)
		scale = 1.0;
	else
		scale = (1.0/(distance - obj->radius - ground_height) + m_abs(1.0/(distance - obj->radius - ground_height)))/2.0;

	if (ref->scale < scale)
		ref->scale = scale;

	double y_angle, x_angle;
	y_angle = asin(vector.y) + m_pi/2.0;
	if ((vector.x/cos(y_angle - m_pi/2.0) > 1.0 || vector.x/cos(y_angle - m_pi/2.0) < -1.0) && vector.z != 0.0 && vector.x != 0.0)
		x_angle = atan(vector.z/vector.x);
	else if (cos(y_angle - m_pi/2.0) != 0.0)
		x_angle = acos(vector.x/cos(y_angle - m_pi/2.0));
	else
		x_angle = 0.0;

	if (vector.z < 0)
		x_angle = 2.0*m_pi - x_angle;

	orient = (180.0/m_pi)*vec(0.0, 2.0*m_pi - x_angle, m_pi - acos(dot(vec(0.0, 1.0, 0.0), vector)));

	if (distance - obj->radius <= ground_height)
	{
		ref->object->orientation = orient;
		radius = obj->radius;

		ref->object->position = vector*(obj->radius + ground_height) + obj->position;
	}

	glPushMatrix();
	glLoadIdentity();
	glScaled(1.0/ref->oldscale, 1.0/ref->oldscale, 1.0/ref->oldscale);
	glRotated(orient.x, 1.0, 0.0, 0.0);
    glRotated(orient.y, 0.0, 1.0, 0.0);
    glRotated(orient.z, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, spheremat);
	glPopMatrix();
}

void cameraref::render()
{
	glPushMatrix();
	glScaled(distance, distance, distance);
	glRotated(orient.x, 1.0, 0.0, 0.0);
    glRotated(orient.y, 0.0, 1.0, 0.0);
    glRotated(orient.z, 0.0, 0.0, 1.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, ref->sphereverts);
	glTexCoordPointer(1, GL_FLOAT, 0, ref->texcoord);
	for (int x = 0; x < ref->xsize-1; x++)
		glDrawElements(GL_TRIANGLE_STRIP, ref->size*2, GL_UNSIGNED_INT, &ref->indices[x*ref->size*2]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}
