/*
	GameMatrix.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 12/24/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "GameMatrix.h"

template <int Y, int X>
inline Matrix<Y, X>::Matrix()
{
	x = X;
	y = Y;
	
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			v[r][c] = 0.0;
}

template <int Y, int X>
inline  Matrix<Y, X>::Matrix(GLfloat val[Y*X])
{
	x = X;
	y = Y;
	int d = 0;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			v[r][c] = val[d++];
}

template <int Y, int X>
void Matrix<Y, X>::InterchangeRows(int row1, int row2)
{
	GLfloat temp[X];
	for (int c = 0; c < X; c++)
		temp[c] = v[row1][c];
	for (int c = 0; c < X; c++)
		v[row1][c] = v[row2][c];
	for (int c = 0; c < X; c++)
		v[row2][c] = temp[c];
}

template <int Y, int X>
void Matrix<Y, X>::ScaleRow(int row, float scale)
{
	for (int c = 0; c < X; c++)
		v[row][c] *= scale;
}

template <int Y, int X>
void Matrix<Y, X>::AddRow(int row1, int row2, float scale)
{
	for (int c = 0; c < X; c++)
		v[row2][c] += v[row1][c]*scale;
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::Inverse()
{
	if (X == Y)
	{
		Matrix<Y, X> m = *this;
		Matrix<Y, X> i;
		for (int x = 0; x < X; x++)
			i.Value[x][x] = 1;
		int c, r;
		
		for (r = 0; r < Y; r++)
			for (c = 0; c < X; c++)
				if (r != c)
				{
					i.AddRow(c, r, -m.v[r][c]/m.v[c][c]);
					m.AddRow(c, r, -m.v[r][c]/m.c[c][c]);
				}
		for (r = 0; r < Y; r++)
		{
			i.ScaleRow(r, GLfloat(1/m.v[r][r]));
			m.ScaleRow(r, GLfloat(1/m.v[r][r]));
		}
		
		return i;
	}
}

template <int Y, int X>
Matrix<X, Y> Matrix<Y, X>::Transpose()
{
	Matrix<X, Y> m;
	
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			m.v[c][r] = v[r][c];
	
	return m;
}

template <int Y, int X>
void Matrix<Y, X>::Normalize()
{
	GLfloat length;

	length = sqrt((v[0][0] * v[0][0]) + (v[1][0] * v[1][0]) + (v[2][0] * v[2][0]));

	if (length == 0.0f)
		length = 1.0f;

	(*this) /= length;
}

template <int Y, int X>
void Matrix<Y, X>::GetValue(GLfloat *value)
{
	int c = 0;
	for (int y = 0; y < Y; y++)
		for (int x = 0; x < X; x++)
		{
			value[c] = v[y][x];
			c++;
		}
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator =(Matrix<Y, X> m)
{
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			v[r][c] = m.v[r][c];
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator +(const Matrix<Y, X> &m)
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] + m.v[r][c];
	return value;
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator -(const Matrix<Y, X> &m)
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] - m.v[r][c];
	return value;
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator +=(const Matrix<Y, X> &m)
{
	(*this) = (*this) + m;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator -=(const Matrix<Y, X> &m)
{
	(*this) = (*this) - m;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator *=(const Matrix<Y, X> &m)
{
	(*this) = (*this) * m;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator /=(const Matrix<Y, X> &m)
{
	(*this) = (*this) / m;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator +(const GLfloat &f)
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] + f;
	return value;
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator -(const GLfloat &f)
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] - f;
	return value;
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator *(const GLfloat &f)
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] * f;
	return value;
}

template <int Y, int X>
Matrix<Y, X> Matrix<Y, X>::operator /(const GLfloat &f)	
{
	Matrix<Y, X> value;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			value.v[r][c] = v[r][c] / f;
	return value;
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator +=(const GLfloat &f)
{
	(*this) = (*this) + f;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator -=(const GLfloat &f)
{
	(*this) = (*this) - f;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator *=(const GLfloat &f)
{
	(*this) = (*this) * f;
	return (*this);
}

template <int Y, int X>
Matrix<Y, X> &Matrix<Y, X>::operator /=(const GLfloat &f)	
{
	(*this) = (*this) / f;
	return (*this);
}

template <int Y, int X>
bool Matrix<Y, X>::operator ==(const Matrix<Y, X> &m)
{
	int count;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			if (v[r][c] == m.v[r][c])
				count++;
	if (count == Y*X)
		return true;
	else
		return false;
}

template <int Y, int X>
bool Matrix<Y, X>::operator !=(const Matrix<Y, X> &m)
{
	int count;
	for (int r = 0; r < Y; r++)
		for (int c = 0; c < X; c++)
			if (v[r][c] == m.v[r][c])
				count++;
	if (count != Y*X)
		return true;
	else
		return false;
}

Vector::Vector(GLfloat X, GLfloat Y, GLfloat Z)
{
	x = X;
	y = Y;
	z = Z;
}

Vector::Vector(GLfloat value[3])
{
	x = value[0];
	y = value[1];
	z = value[2];
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
	
	return (*this);
}

Vector Vector::operator +(const Vector &v)
{
	return Vector(x+v.x, y+v.y, z+v.z);
}

Vector Vector::operator -(const Vector &v)
{
	return Vector(x-v.x, y-v.y, z-v.z);
}

Vector Vector::operator *(const Vector &v)
{
	return Vector(x*v.x, y*v.y, z*v.z);
}

Vector Vector::operator /(const Vector &v)
{
	if (v.x != 0 && v.y != 0 && v.z != 0)
		return Vector(x/v.x, y/v.y, z/v.z);
	else
		return Vector(x, y, z);
}

Vector &Vector::operator +=(const Vector &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	
	return (*this);
}

Vector &Vector::operator -=(const Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	
	return (*this);
}

Vector &Vector::operator *=(const Vector &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	
	return (*this);
}

Vector &Vector::operator /=(const Vector &v)
{
	if (v.x != 0 && v.y != 0 && v.z != 0)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}
	return (*this);
}

Vector Vector::operator +(const GLfloat &f)
{
	return Vector(x+f, y+f, z+f);
}

Vector Vector::operator -(const GLfloat &f)
{
	return Vector(x-f, y-f, z-f);
}

Vector Vector::operator *(const GLfloat &f)
{
	return Vector(x*f, y*f, z*f);
}

Vector Vector::operator /(const GLfloat &f)
{
	if (f == 0)
		return Vector(x, y, z);
	
	return Vector(x/f, y/f, z/f);
}

Vector &Vector::operator +=(const GLfloat &f)
{
	x += f;
	y += f;
	z += f;
	
	return (*this);
}

Vector &Vector::operator -=(const GLfloat &f)
{
	x -= f;
	y -= f;
	z -= f;
	
	return (*this);
}

Vector &Vector::operator *=(const GLfloat &f)
{
	x *= f;
	y *= f;
	z *= f;
	
	return (*this);
}

Vector &Vector::operator /=(const GLfloat &f)

{
	if (f == 0)
		return (*this);
	
	x /= f;
	y /= f;
	z /= f;
	
	return (*this);
}

bool Vector::operator ==(const Vector &m)
{
	if (x == m.x && y == m.y && z == m.z)
		return true;
	else
		return false;
}

bool Vector::operator !=(const Vector &m)
{
	if (x != m.x || y != m.y || z != m.z)
		return true;
	else
		return false;
}

bool Vector::operator <=(const Vector &m)
{
	if (x <= m.x && y <= m.y && z <= m.z)
		return true;
	else
		return false;
}

bool Vector::operator >=(const Vector &m)
{
	if (x >= m.x && y >= m.y && z >= m.z)
		return true;
	else
		return false;
}

void Vector::Normalize()
{
	GLfloat length;

	length = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));

	if (length == 0.0f)
		length = 1.0f;

	(*this) /= length;
}

Vector Vector::GetNorm()
{
	GLfloat length;

	length = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));

	if (length == 0.0f)
		length = 1.0f;

	return ((*this) / length);
}

Vector Vector::Cross(const Vector &v)
{
	return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

GLfloat Vector::Dot(const Vector &v)
{
	return (x*v.x + y*v.y + z*v.z);
}

Vector Vector::Reflect(const Vector &v)
{
	GLfloat dot = (*this).Dot(v);
	return ((*this) - Vector(v.x, v.y, v.z)*dot*2);
}

GLfloat Vector::Length()
{
	return sqrt((x * x) + (y * y) + (z * z));
}
