/*
 *  Matrix.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Matrix.h"

void SetMatrix(Matrix Input, Matrix Output)
{
	for (int x = 0; x < 16; x++)
		Output[x] = Input[x];
}

void InterchangeRows(Matrix M, int row1, int row2)
{
	GLdouble temp[4];
	for (int x = 0; x < 4; x++)
		temp[x] = M[(x*4) + row1];
	for (int x = 0; x < 4; x++)
		M[(x*4) + row1] = M[(x*4) + row2];
	for (int x = 0; x < 4; x++)
		M[(x*4) + row2] = temp[x];
}

void ScaleRow(Matrix M, int row, GLdouble scale)
{
	for (int x = 0; x < 4; x++)
		M[(x*4) + row] *= scale;
}

void AddRow(Matrix M, int row1, int row2, GLdouble scale)
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
		ScaleRow(i, x, GLdouble(1/M[(x*4) + x]));
		ScaleRow(M, x, GLdouble(1/M[(x*4) + x]));
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

void Mult(Matrix Result, Matrix Input1, Matrix Input2)
{
	for (int x = 0; x < 16; x++)
		Result[x] = 0;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			for (int z = 0; z < 4; z++)
				Result[(x*4) + y] += Input1[(z*4) + y]*Input2[(x*4) + z];
}
