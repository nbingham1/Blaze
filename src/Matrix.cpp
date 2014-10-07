/*
	Matrix.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/16/06.
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

#include "Matrix.h"

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

