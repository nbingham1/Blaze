/*
	Vector.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/7/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Vector.h"

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

