/*
 *  Matrix.h
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/25/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#ifndef Matrix_h
#define Matrix_h

#include "OpenGLIncludes.h"
#include <math.h>

typedef GLdouble Matrix[16];

void SetMatrix(Matrix Input, Matrix Output);

void InterchangeRows(Matrix M, int row1, int row2);
void ScaleRow(Matrix M, int row, GLdouble scale);
void AddRow(Matrix M, int row1, int row2, GLdouble scale);

void Inverse(Matrix M);
void Transpose(Matrix M);

void Mult(Matrix Result, Matrix Input1, Matrix Input2);

#endif