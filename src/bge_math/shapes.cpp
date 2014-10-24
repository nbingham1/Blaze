#include "shapes.h"
#include "../common.h"

planehdl::planehdl()
{
}

planehdl::planehdl(vec v1, vec v2, vec v3)
{
	normal = norm(cross(v2 - v1, v3 - v1));

	offset = dot(normal, v1);
}

planehdl::~planehdl()
{

}

void planehdl::generate(vec v1, vec v2, vec v3)
{
	normal = norm(cross(v2 - v1, v3 - v1));

	offset = dot(normal, v1);
}

double planehdl::distance(vec v)
{
	return dot(normal, v) + offset;
}

frustumhdl::frustumhdl()
{
	leftf = 0.0;
	rightf = 0.0;
	downf = 0.0;
	upf = 0.0;
	nearf = 0.0;
	farf = 0.0;
}

frustumhdl::~frustumhdl()
{
	leftf = 0.0;
	rightf = 0.0;
	downf = 0.0;
	upf = 0.0;
	nearf = 0.0;
	farf = 0.0;
}

void frustumhdl::initialize(double l, double r, double d, double u, double n, double f)
{
	leftf = l;
	rightf = r;
	downf = d;
	upf = u;
	nearf = n;
	farf = f;

	vec v[] = {vec(0.0, 0.0, 0.0),
			   vec(leftf, downf, nearf),
			   vec(rightf, downf, nearf),
			   vec(rightf, upf, nearf),
			   vec(leftf, upf, nearf),
			   vec(leftf, downf, farf),
			   vec(rightf, downf, farf),
			   vec(rightf, upf, farf),
			   vec(leftf, upf, farf)};

	base[0] = norm(cross(v[1] - v[0], v[4] - v[0]));
	base[1] = norm(cross(v[4] - v[0], v[3] - v[0]));
	base[2] = norm(cross(v[3] - v[0], v[2] - v[0]));
	base[3] = norm(cross(v[2] - v[0], v[1] - v[0]));
	base[4] = norm(cross(v[2] - v[3], v[1] - v[3]));
	base[5] = norm(cross(v[6] - v[7], v[4] - v[7]));
}

void frustumhdl::set_orientation(vec angle)
{
	vec o = degtorad(angle);
	vec x = rotz(roty(rotx(vec(1.0, 0.0, 0.0), o.x), o.y), o.z);
	vec y = rotz(roty(rotx(vec(0.0, 1.0, 0.0), o.x), o.y), o.z);
	vec z = rotz(roty(rotx(vec(0.0, 0.0, 1.0), o.x), o.y), o.z);

	for (int a = 0; a < 6; a++)
		transform[a] = base[a].x*x + base[a].y*y + base[a].z*z;
}

void frustumhdl::rotate(vec angle)
{
	vec o = degtorad(angle);
	vec x = rotz(roty(rotx(vec(1.0, 0.0, 0.0), o.x), o.y), o.z);
	vec y = rotz(roty(rotx(vec(0.0, 1.0, 0.0), o.x), o.y), o.z);
	vec z = rotz(roty(rotx(vec(0.0, 0.0, 1.0), o.x), o.y), o.z);

	for (int a = 0; a < 6; a++)
		transform[a] = transform[a].x*x + transform[a].y*y + transform[a].z*z;
}

double frustumhdl::dist(vec point)
{
	double distance = 0.0;

	for (int a = 0; a < 6; a++)
		distance = m_max(distance, dot(point, transform[a]));

	return distance;
}

space::space()
{
	x = vec(1.0, 0.0, 0.0);
	y = vec(0.0, 1.0, 0.0);
	z = vec(0.0, 0.0, 1.0);
}

space::~space()
{
}

void space::rotate_xyz(vec angle)
{
	x = rotz(roty(rotx(x, angle.x), angle.y), angle.z);
	y = rotz(roty(rotx(y, angle.x), angle.y), angle.z);
	z = rotz(roty(rotx(z, angle.x), angle.y), angle.z);
}

void space::rotate_zyx(vec angle)
{
	x = rotx(roty(rotz(x, angle.z), angle.y), angle.x);
	y = rotx(roty(rotz(y, angle.z), angle.y), angle.x);
	z = rotx(roty(rotz(z, angle.z), angle.y), angle.x);
}

void space::orient_xyz(vec angle)
{
	x = rotz(roty(rotx(vec(1.0, 0.0, 0.0), angle.x), angle.y), angle.z);
	y = rotz(roty(rotx(vec(0.0, 1.0, 0.0), angle.x), angle.y), angle.z);
	z = rotz(roty(rotx(vec(0.0, 0.0, 1.0), angle.x), angle.y), angle.z);
}

void space::orient_zyx(vec angle)
{
	x = rotx(roty(rotz(vec(1.0, 0.0, 0.0), angle.z), angle.y), angle.x);
	y = rotx(roty(rotz(vec(0.0, 1.0, 0.0), angle.z), angle.y), angle.x);
	z = rotx(roty(rotz(vec(0.0, 0.0, 1.0), angle.z), angle.y), angle.x);
}

vec space::transform(vec vector)
{
	return vector.x*x + vector.y*y + vector.z*z;
}

sphere::sphere()
{
}

sphere::sphere(vec c, double r)
{
	center = c;
	radius = r;
}

sphere::~sphere()
{
}

bool sphere::intersect_ray(vec r, vec *p, vec *n)
{
	double a = dot(r, center),
		   b = a*a - dot(center, center) + radius*radius;

	if (p != NULL)
		*p = (a + sqrt(b))*r;
	if (n != NULL)
		*n = (a - sqrt(b))*r;

	return b > 0.0;
}

int intersect_ray_triangle(vec orig, vec dir, vec vert0, vec vert1, vec vert2, double *t, double *u, double *v)
{
	bool cull = true;
	vec edge1, edge2, tvec, pvec, qvec;
	double det, inv_det;
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	pvec = cross(dir, edge2);
	det = dot(edge1, pvec);

	if (cull)
	{
		if (det < m_epsilon)
			return 0;
		tvec = orig - vert0;
		*u = dot(tvec, pvec);
		if (*u < 0.0 || *u > det)
			return 0;
		qvec = cross(tvec, edge1);
		*v = dot(dir, qvec);
		if (*v < 0.0 || *u + *v > det)
			return 0;
		*t = dot(edge2, qvec);
		inv_det = 1.0 / det;
		*t *= inv_det;
		*u *= inv_det;
		*v *= inv_det;
	}
	else
	{
		if (det > -m_epsilon && det < m_epsilon)
			return 0;
		inv_det = 1.0 / det;
		tvec = orig - vert0;
		*u = dot(tvec, pvec) * inv_det;
		if (*u < -m_epsilon || *u > 1.0 + m_epsilon)
			return 0;
		qvec = cross(tvec, edge1);
		*v = dot(dir, qvec) * inv_det;
		if (*v < -m_epsilon || *u + *v > 1.0 + m_epsilon)
			return 0;
		*t = dot(edge2, qvec) * inv_det;
	}
	return 1;
}
