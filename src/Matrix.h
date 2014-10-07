/*
	Matrix.h
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

#include "BasicDefinitions.h"

#ifndef Matrix_h
#define Matrix_h

typedef GLfloat Matrix[16];

void SetMatrix(Matrix Input, Matrix Output);

void InterchangeRows(Matrix M, int row1, int row2);
void ScaleRow(Matrix M, int row, GLfloat scale);
void AddRow(Matrix M, int row1, int row2, GLfloat scale);

void Inverse(Matrix M);
void Transpose(Matrix M);

#endif
