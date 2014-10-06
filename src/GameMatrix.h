/*
	GameMatrix.h
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

#include "Main.h"

#ifndef Matrix_h
#define Matrix_h

#define pi 3.1415926535898

template <int Y, int X>
struct Matrix
{
	Matrix();
	Matrix(GLfloat val[Y*X]);
	
	int x, y;
	GLfloat v[Y][X];
	
	void InterchangeRows(int row1, int row2);
	void ScaleRow(int row, GLfloat scale);
	void AddRow(int row1, int row2, GLfloat scale);
	
	Matrix<Y, X> Inverse();
	Matrix<X, Y> Transpose();
	void Normalize();
	
	void GetValue(GLfloat *value);
	
	Matrix<Y, X> &operator =(Matrix<Y, X> m);
	
	Matrix<Y, X> operator +(const Matrix<Y, X> &m);
	Matrix<Y, X> operator -(const Matrix<Y, X> &m);
	template <int Y1, int X1>
	Matrix<Y, X1> operator *(const Matrix<Y1, X1> &m)
	{
		Matrix<Y, X1> value;
		for (int r = 0; r < Y; r++)
			for (int c = 0; c < X1; c++)
				for (int w = 0; w < X; w++)
					value.v[r][c] += v[r][w]*m.v[c][w];
		return value;
	}
	template <int Y1, int X1>
	Matrix<Y, X1> operator /(const Matrix<Y1, X1> &m)
	{
		Matrix<Y, X1> value = (*this)*m.Inverse();
		return value;
	}
	
	Matrix<Y, X> &operator +=(const Matrix<Y, X> &m);
	Matrix<Y, X> &operator -=(const Matrix<Y, X> &m);
	Matrix<Y, X> &operator *=(const Matrix<Y, X> &m);
	Matrix<Y, X> &operator /=(const Matrix<Y, X> &m);
	
	Matrix<Y, X> operator +(const GLfloat &f);
	Matrix<Y, X> operator -(const GLfloat &f);
	Matrix<Y, X> operator *(const GLfloat &f);
	Matrix<Y, X> operator /(const GLfloat &f);
	
	Matrix<Y, X> &operator +=(const GLfloat &f);
	Matrix<Y, X> &operator -=(const GLfloat &f);
	Matrix<Y, X> &operator *=(const GLfloat &f);
	Matrix<Y, X> &operator /=(const GLfloat &f);
	
	bool operator ==(const Matrix<Y, X> &m);
	bool operator !=(const Matrix<Y, X> &m);
};

struct Vector
{
	Vector(GLfloat X, GLfloat Y, GLfloat Z);
	Vector(GLfloat value[3]);
	Vector();
	~Vector();
	
	GLfloat x, y, z;
	
	Vector &operator =(Vector v);
	
	Vector operator +(const Vector &v);
	Vector operator -(const Vector &v);
	Vector operator *(const Vector &v);
	Vector operator /(const Vector &v);	
	
	Vector &operator +=(const Vector &v);
	Vector &operator -=(const Vector &v);
	Vector &operator *=(const Vector &v);
	Vector &operator /=(const Vector &v);
	
	Vector operator +(const GLfloat &f);
	Vector operator -(const GLfloat &f);
	Vector operator *(const GLfloat &f);
	Vector operator /(const GLfloat &f);
	
	Vector &operator +=(const GLfloat &f);
	Vector &operator -=(const GLfloat &f);
	Vector &operator *=(const GLfloat &f);
	Vector &operator /=(const GLfloat &f);
	
	bool operator ==(const Vector &m);
	bool operator !=(const Vector &m);
	bool operator <=(const Vector &m);
	bool operator >=(const Vector &m);
		
	void Normalize();
	Vector GetNorm();
	Vector Cross(const Vector &v);
	GLfloat Dot(const Vector &v);
	Vector Reflect(const Vector &v);
	GLfloat Length();
};

#endif
