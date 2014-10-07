/*
	GeometricalTypes.cpp
	Blaze Game Engine 0.02

	Created by Ned Bingham on 4/13/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.02 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.02 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.02.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GeometricalTypes.h"

/********************************************
 *											*
 *											*
 *				***Matrices***				*
 *											*
 *											*
 ********************************************/

void SetMatrix(Matrix Input, Matrix Output)
{
	for (int x = 0; x < 16; x++)
		Output[x] = Input[x];
}

void InterchangeRows(Matrix M, int row1, int row2)
{
	GLfloat temp[4];
	for (int x = 0; x < 4; x++)
		temp[x] = M[(x*4) + row1];
	for (int x = 0; x < 4; x++)
		M[(x*4) + row1] = M[(x*4) + row2];
	for (int x = 0; x < 4; x++)
		M[(x*4) + row2] = temp[x];
}

void ScaleRow(Matrix M, int row, GLfloat scale)
{
	for (int x = 0; x < 4; x++)
		M[(x*4) + row] *= scale;
}

void AddRow(Matrix M, int row1, int row2, GLfloat scale)
{
	for (int x = 0; x < 4; x++)
		M[(x*4) + row1] += M[(x*4) + row2]*scale;
}

void Inverse(Matrix M)
{
	Matrix i;
	for (int x = 0; x < 16; x++)
		i[x] = 0;
	for (int x = 0; x < 4; x++)
		i[x*5] = 1;
	
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			if (x != y)
			{
				AddRow(i, x, y, -M[(x*4) + y]/M[(y*4) + y]);
				AddRow(M, x, y, -M[(x*4) + y]/M[(y*4) + y]);
			}
	for (int x = 0; x < 4; x++)
	{
		ScaleRow(i, x, GLfloat(1/M[(x*4) + x]));
		ScaleRow(M, x, GLfloat(1/M[(x*4) + x]));
	}
	
	SetMatrix(i, M);
}

void Transpose(Matrix M)
{
	Matrix m;
	SetMatrix(M, m);
	
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			M[(x*4) + y] = m[(y*4) + x];
}

/********************************************
 *											*
 *											*
 *				***Vector***				*
 *											*
 *											*
 ********************************************/

Vector::Vector(GLfloat X, GLfloat Y, GLfloat Z)
{
	x = X;
	y = Y;
	z = Z;
}

Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::~Vector()
{
}

Vector &Vector::operator =(Vector v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vector &Vector::operator +=(Vector v)
{
	*this = *this + v;
	return *this;
}

Vector &Vector::operator -=(Vector v)
{
	*this = *this - v;
	return *this;
}

Vector &Vector::operator *=(Vector v)
{
	*this = *this * v;
	return *this;
}

Vector &Vector::operator /=(Vector v)
{
	*this = *this / v;
	return *this;
}

Vector &Vector::operator +=(GLfloat f)
{
	*this = *this + f;
	return *this;
}

Vector &Vector::operator -=(GLfloat f)
{
	*this = *this - f;
	return *this;
}

Vector &Vector::operator *=(GLfloat f)
{
	*this = *this * f;
	return *this;
}

Vector &Vector::operator /=(GLfloat f)
{
	*this = *this / f;
	return *this;
}

/********************************************
 *											*
 *											*
 *			   ***EulerAngle***		    	*
 *											*
 *											*
 ********************************************/

EulerAngle::EulerAngle(GLfloat X, GLfloat Y, GLfloat Z)
{
	x = X;
	y = Y;
	z = Z;
}

EulerAngle::EulerAngle()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

EulerAngle::~EulerAngle()
{
}

EulerAngle &EulerAngle::operator =(EulerAngle a)
{
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
	return *this;
}

EulerAngle &EulerAngle::operator =(Vector v)
{
	this->x = (v.y-v.x+v.z)/2;
	this->y = (v.y+v.x-v.z)/2;
	this->z = (-v.y+v.x+v.z)/2;
	return *this;
}

EulerAngle &EulerAngle::operator +=(EulerAngle a)
{
	*this = *this + a;
	return *this;
}

EulerAngle &EulerAngle::operator -=(EulerAngle a)
{
	*this = *this - a;
	return *this;
}

EulerAngle &EulerAngle::operator *=(EulerAngle a)
{
	*this = *this * a;
	return *this;
}

EulerAngle &EulerAngle::operator /=(EulerAngle a)
{
	*this = *this / a;
	return *this;
}

EulerAngle &EulerAngle::operator +=(GLfloat f)
{
	*this = *this + f;
	return *this;
}

EulerAngle &EulerAngle::operator -=(GLfloat f)
{
	*this = *this - f;
	return *this;
}

EulerAngle &EulerAngle::operator *=(GLfloat f)
{
	*this = *this * f;
	return *this;
}

EulerAngle &EulerAngle::operator /=(GLfloat f)
{
	*this = *this / f;
	return *this;
}

void EulertoMatrix(EulerAngle e, GLfloat m[16])
{
	GLfloat c1 = cos(e.x), c2 = cos(e.y), c3 = cos(e.z);
	GLfloat s1 = sin(e.x), s2 = sin(e.y), s3 = sin(e.z);
	
	m[0] = c2*c1;
	m[1] = s1;
	m[2] = -s2*c1;
	m[3] = 0;
	m[4] = -c2*s1*c3 + s2*s3;
	m[5] = c1*c3;
	m[6] = s2*s1*c3 + c2*s3;
	m[7] = 0;
	m[8] = c2*s1*s3 + s2*c3;
	m[9] = -c1*s3;
	m[10] = -s2*s1*s3 + c2*c3;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;
}

Vector EulertoVector(EulerAngle e)
{
	return Vector(-(e.y + e.z), -(e.x + e.z), -(e.x + e.y));
}

/********************************************
 *											*
 *											*
 *			   ***AxisAngle***				*
 *											*
 *											*
 ********************************************/

AxisAngle::AxisAngle(Vector a, GLfloat r)
{
	axis = a;
	angle = r;
}

AxisAngle::AxisAngle()
{
	axis = Vector(0.0, 0.0, 0.0);
	angle = 0.0;
}

AxisAngle::~AxisAngle()
{
}

AxisAngle &AxisAngle::operator =(AxisAngle a)
{
	this->axis = a.axis;
	this->angle = a.angle;
	return *this;
}

AxisAngle &AxisAngle::operator =(EulerAngle a)
{
	GLfloat c1 = cos(a.y/2), c2 = cos(a.x/2), c3 = cos(a.z/2);
	GLfloat s1 = sin(a.y/2), s2 = sin(a.x/2), s3 = sin(a.z/2);
	
	this->angle = 2*acos(c1*c2*c3 - s1*s2*s3);
	this->axis.x = s1*s2*c3 + c1*c2*s3;
	this->axis.y = s1*c2*c3 + c1*s2*s3;
	this->axis.z = c1*s2*c3 - s1*c2*s3;
	this->axis = Normalize(this->axis);
	return *this;
}

AxisAngle &AxisAngle::operator +=(AxisAngle a)
{
	*this = *this + a;
	return *this;
}

AxisAngle &AxisAngle::operator -=(AxisAngle a)
{
	*this = *this - a;
	return *this;
}

AxisAngle &AxisAngle::operator *=(AxisAngle a)
{
	*this = *this * a;
	return *this;
}

AxisAngle &AxisAngle::operator /=(AxisAngle a)
{
	*this = *this / a;
	return *this;
}

AxisAngle &AxisAngle::operator +=(EulerAngle a)
{
	*this = *this + a;
	return *this;
}

AxisAngle &AxisAngle::operator -=(EulerAngle a)
{
	*this = *this - a;
	return *this;
}

AxisAngle &AxisAngle::operator *=(EulerAngle a)
{
	*this = *this * a;
	return *this;
}

AxisAngle &AxisAngle::operator /=(EulerAngle a)
{
	*this = *this / a;
	return *this;
}

AxisAngle &AxisAngle::operator +=(GLfloat f)
{
	this->angle = this->angle + f;
	return *this;
}

AxisAngle &AxisAngle::operator -=(GLfloat f)
{
	this->angle = this->angle - f;
	return *this;
}

AxisAngle &AxisAngle::operator *=(GLfloat f)
{
	this->angle = this->angle * f;
	return *this;
}

AxisAngle &AxisAngle::operator /=(GLfloat f)
{
	this->angle = this->angle / f;
	return *this;
}

EulerAngle AxistoEuler(AxisAngle a)
{
	GLfloat s = sin(a.angle);
	GLfloat c = cos(a.angle);
	GLfloat t = 1 - c;
	GLfloat x, y, z;
	if ((a.axis.x*a.axis.y*t + a.axis.z*s) >= 1)
	{
		y = 2*atan2(a.axis.x*sin(a.angle/2), cos(a.angle/2));
		x = pi/2;
		z = 0;
		return EulerAngle(x, y, z);
	}
	if ((a.axis.x*a.axis.y*t + a.axis.z*s) <= -1)
	{
		y = -2*atan2(a.axis.x*sin(a.angle/2), cos(a.angle/2));
		x = -pi/2;
		z = 0;
		return EulerAngle(x, y, z);
	}
	
	y = atan2(a.axis.y * s- a.axis.x * a.axis.z * t , 1 - (a.axis.y*a.axis.y+ a.axis.z*a.axis.z ) * t);
	x = asin(a.axis.x * a.axis.y * t + a.axis.z * s);
	z = atan2(a.axis.x * s - a.axis.y * a.axis.z * t , 1 - (a.axis.x*a.axis.x + a.axis.z*a.axis.z) * t);
	
	return EulerAngle(x, y, z);
}

void AxistoMatrix(AxisAngle a, GLfloat m[16])
{
	GLfloat c = cos(a.angle), s = sin(a.angle), t = 1 - c;
	GLfloat x = a.axis.x, y = a.axis.y, z = a.axis.z;
	
	m[0] = t*x*x + c;
	m[1] = t*x*y + z*s;
	m[2] = t*x*z - y*s;
	m[3] = 0;
	m[4] = t*x*y - z*s;
	m[5] = t*y*y + c;
	m[6] = t*y*z + x*s;
	m[7] = 0;
	m[8] = t*x*z + y*s;
	m[9] = t*y*z - x*s;
	m[10] = t*z*z + c;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;
}

/********************************************
 *											*
 *											*
 *		   	   ***Quaternion***		    	*
 *											*
 *											*
 ********************************************/

Quaternion::Quaternion(GLfloat R, Vector I)
{
	r = R;
	i = I.x;
	j = I.y;
	k = I.z;
}

Quaternion::Quaternion(GLfloat R, GLfloat I, GLfloat J, GLfloat K)
{
	r = R;
	i = I;
	j = J;
	k = K;
}

Quaternion::Quaternion()
{
	r = 0.0;
	i = 0.0;
	j = 0.0;
	k = 0.0;
}

Quaternion::~Quaternion()
{
}

Quaternion &Quaternion::operator =(Quaternion q)
{
	this->r = q.r;
	this->i = q.i;
	this->j = q.j;
	this->k = q.k;
	return *this;
}

Quaternion &Quaternion::operator =(AxisAngle a)
{
	this->r = cos(a.angle/2);
	this->i = a.axis.x * sin(a.angle/2);
	this->j = a.axis.y * sin(a.angle/2);
	this->k = a.axis.z * sin(a.angle/2);
	return *this;
}

Quaternion &Quaternion::operator =(EulerAngle a)
{
	GLfloat c1 = cos(a.y/2), c2 = cos(a.x/2), c3 = cos(a.z/2);
	GLfloat s1 = sin(a.y/2), s2 = sin(a.x/2), s3 = sin(a.z/2);
	this->r = c1*c2*c3 - s1*s2*s3;
	this->i = s1*s2*c3 + c1*c2*s3;
	this->j = s1*c2*c3 + c1*s2*s3;
	this->k = c1*s2*c3 - s1*c2*s3;
	return *this;
}

Quaternion &Quaternion::operator +=(Quaternion q)
{
	*this = *this + q;
	return *this;
}

Quaternion &Quaternion::operator -=(Quaternion q)
{
	*this = *this - q;
	return *this;
}

Quaternion &Quaternion::operator *=(Quaternion q)
{
	*this = *this * q;
	return *this;
}

Quaternion &Quaternion::operator /=(Quaternion q)
{
	*this = *this / q;
	return *this;
}

Quaternion &Quaternion::operator +=(AxisAngle a)
{
	*this = *this + a;
	return *this;
}

Quaternion &Quaternion::operator -=(AxisAngle a)
{
	*this = *this - a;
	return *this;
}

Quaternion &Quaternion::operator *=(AxisAngle a)
{
	*this = *this * a;
	return *this;
}

Quaternion &Quaternion::operator /=(AxisAngle a)
{
	*this = *this / a;
	return *this;
}

Quaternion &Quaternion::operator +=(EulerAngle a)
{
	*this = *this + a;
	return *this;
}

Quaternion &Quaternion::operator -=(EulerAngle a)
{
	*this = *this - a;
	return *this;
}

Quaternion &Quaternion::operator *=(EulerAngle a)
{
	*this = *this * a;
	return *this;
}

Quaternion &Quaternion::operator /=(EulerAngle a)
{
	*this = *this / a;
	return *this;
}

EulerAngle QuaterniontoEuler(Quaternion q)
{
    GLfloat sqw = q.r*q.r;
    GLfloat sqx = q.i*q.i;
    GLfloat sqy = q.j*q.j;
    GLfloat sqz = q.k*q.k;
	GLfloat unit = sqx + sqy + sqz + sqw;
	GLfloat test = q.i*q.j + q.k*q.r;
	
	GLfloat y;
	GLfloat x;
	GLfloat z;
	
	if (test >= 0.5*unit)
	{
		y = 2 * atan2(q.i,q.r);
		x = pi/2;
		z = 0;
		return EulerAngle(x, y, z);
	}
	if (test <= -0.5*unit)
	{
		y = -2 * atan2(q.i,q.r);
		x = -pi/2;
		z = 0;
		return EulerAngle(x, y, z);
	}
    y = atan2(2*q.j*q.r-2*q.i*q.k , sqx - sqy - sqz + sqw);
	x = asin(2*test/unit);
	z = atan2(2*q.i*q.r-2*q.j*q.k , -sqx + sqy - sqz + sqw);
	return EulerAngle(x, y, z);
}

AxisAngle QuaterniontoAxis(Quaternion q)
{
	if (q.r > 1)
		Normalize(q);
		
	GLfloat angle = 2 * acos(q.r);
	GLfloat x, y, z;
	GLfloat s = sqrt(1-q.r*q.r);
	if (s < 0.001)
	{
		x = q.i;
		y = q.j;
		z = q.k;
	}
	else
	{
		x = q.i / s;
		y = q.j / s;
		z = q.k / s;
	}
	
	return AxisAngle(Vector(x, y, z), angle);
}

/********************************************
 *											*
 *											*
 *			***Simple Geometries***		   	*
 *											*
 *											*
 ********************************************/

Triangle &Triangle::operator =(Triangle t)
{
	this->v[0] = t.v[0];
	this->v[1] = t.v[1];
	this->v[2] = t.v[2];
	
	this->CollisionType = t.CollisionType;
	this->Next = t.Next;
	return *this;
}

void AddTriangle(Triangle *Data, Triangle *t)
{
	Triangle *Current = Data;
	while (Current->Next != NULL)
		Current = Current->Next;
	Current->Next = t;
}

void ReleaseTriangles(Triangle *t)
{
	Triangle *Current = t, *Previous;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		free(Previous);
	}
}

GLfloat AreaOfTriangle(Triangle t)
{
	GLfloat a, b, c, s;
	a = sqrt((t.v[0].x-t.v[1].x)*(t.v[0].x-t.v[1].x) + (t.v[0].y-t.v[1].y)*(t.v[0].y-t.v[1].y) + (t.v[0].z-t.v[1].z)*(t.v[0].z-t.v[1].z));
	b = sqrt((t.v[1].x-t.v[2].x)*(t.v[1].x-t.v[2].x) + (t.v[1].y-t.v[2].y)*(t.v[1].y-t.v[2].y) + (t.v[1].z-t.v[2].z)*(t.v[1].z-t.v[2].z));
	c = sqrt((t.v[2].x-t.v[0].x)*(t.v[2].x-t.v[0].x) + (t.v[2].y-t.v[0].y)*(t.v[2].y-t.v[0].y) + (t.v[2].z-t.v[0].z)*(t.v[2].z-t.v[0].z));
	s = (a + b + c)/2;
	
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

void CreateBox(Vertex Max, Vertex Min, Box *b)
{
	b->v[0] = Min;
	b->v[1] = Vector(Min.x, Min.y, Max.z);
	b->v[2] = Vector(Max.x, Min.y, Max.z);
	b->v[3] = Vector(Max.x, Min.y, Min.z);
	b->v[4] = Vector(Min.x, Max.y, Min.z);
	b->v[5] = Vector(Min.x, Max.y, Max.z);
	b->v[6] = Max;
	b->v[7] = Vector(Max.x, Max.y, Min.z);
	
	b->edges[0][0] = 0;
	b->edges[0][1] = 1;
	b->edges[1][0] = 1;
	b->edges[1][1] = 2;
	b->edges[2][0] = 2;
	b->edges[2][1] = 3;
	b->edges[3][0] = 3;
	b->edges[3][1] = 0;
	
	b->edges[4][0] = 4;
	b->edges[4][1] = 5;
	b->edges[5][0] = 5;
	b->edges[5][1] = 6;
	b->edges[6][0] = 6;
	b->edges[6][1] = 7;
	b->edges[7][0] = 7;
	b->edges[7][1] = 4;
	
	b->edges[8][0] = 0;
	b->edges[8][1] = 4;
	b->edges[9][0] = 1;
	b->edges[9][1] = 5;
	b->edges[10][0] = 2;
	b->edges[10][1] = 6;
	b->edges[11][0] = 3;
	b->edges[11][1] = 7;
	
	b->faces[0][0] = 0;
	b->faces[0][1] = 1;
	b->faces[0][2] = 2;
	b->faces[0][3] = 3;
	
	b->faces[1][0] = 4;
	b->faces[1][1] = 5;
	b->faces[1][2] = 6;
	b->faces[1][3] = 7;
	
	b->faces[2][0] = 0;
	b->faces[2][1] = 4;
	b->faces[2][2] = 5;
	b->faces[2][3] = 1;
	
	b->faces[3][0] = 1;
	b->faces[3][1] = 5;
	b->faces[3][2] = 6;
	b->faces[3][3] = 2;
	
	b->faces[4][0] = 2;
	b->faces[4][1] = 6;
	b->faces[4][2] = 7;
	b->faces[4][3] = 3;
	
	b->faces[5][0] = 3;
	b->faces[5][1] = 7;
	b->faces[5][2] = 4;
	b->faces[5][3] = 0;
}

void AddGeometries(Box b, Triangle t, Vertex Output[24])
{
	int o = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Output[o] = b.v[x] + t.v[y];
			o++;
		}
	}
}

Plane &Plane::operator =(Plane p)
{
	this->Normal = p.Normal;
	this->Distance = p.Distance;
	return *this;
}

Plane TriangleToPlane(Vertex v1, Vertex v2, Vertex v3)
{
	Plane p;
	p.Normal = CalculateNormal(v1, v2, v3);
	p.Distance = -(Dot(p.Normal, v1));
	return p;
}

int ClassifyVertextoPlane(Plane p, Vertex v)
{
	GLfloat D = Dot(v, p.Normal) + p.Distance;
	if (D > 0.00000001)
		return 0;
	else if (D < -0.00000001)
		return 1;
	else
		return 2;
}

int ClassifyTriangletoPlane(Plane p, Triangle *t)
{
	int points[] = {0, 0, 0};
	for (int x = 0; x < 3; x++)
		points[ClassifyVertextoPlane(p, t->v[x])]++;
	
	if (points[1] == 0)
		return 0;
	else if (points[0] == 0)
		return 1;
	else if (points[2] == 3)
		return 2;
	else
		return 3;
}

int intersect_RayTriangle(Vector R1, Vector R2, GLfloat radius, Vector T1, Vector T2, Vector T3, Vector *I, Vector *Normal)
{
    Vector    u, v, n;
    Vector    dir, w0, w;
    GLfloat     r, a, b;
	
    u = T2 - T1;
    v = T3 - T1;
    n = Cross(u, v);
    if (n == Vector(0, 0, 0))
        return -1;

    dir = R2 - R1;
    w0 = R1 - T1;
    a = -Dot(n, w0);
    b = Dot(n, dir);
	*Normal = n;
    if (fabs(b) < 0.00000001)
	{
        if (a == 0)
            return 2;
        else return 0;
    }

    r = a / b;
    if (r < 0.0)
		return 0;
	if (r > 1.0)
		return 0;  

    *I = R1 + dir * r;

    GLfloat    uu, uv, vv, wu, wv, D;
    uu = Dot(u, u);
    uv = Dot(u, v);
    vv = Dot(v, v);
    w = *I - T1;
    wu = Dot(w, u);
    wv = Dot(w, v);
    D = uv * uv - uu * vv;

    GLfloat s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0)
        return 3;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return 3;

    return 1;
}

int intersect_RayPlane(Vector R1, Vector R2, Plane p, Vector *I)
{
	Vector    u, v, n;
    Vector    dir, w0, w;
    GLfloat     r, a, b;

    dir = R2 - R1;
    w0 = R1 - Vertex(0.0, p.Distance, 0.0);
    a = -Dot(p.Normal, w0);
    b = Dot(p.Normal, dir);

    if (fabs(b) < 0.00000001)
	{
        if (a == 0)
		{
            return 2;
		}
        else
		{
			return 0;
		}
    }

    r = a / b;
    if (r < 0.0)
	{
		return 0;
	}
	if (r > 1.0)
	{
		return 0;  
	}

    *I = R1 + dir * r;
	
	return 1;
}
