/*
 * frame.h
 *
 *  Created on: May 24, 2015
 *      Author: nbingham
 */

#include <math/matrix_stack.h>

using namespace core;

#ifndef frame_h
#define frame_h

struct framehdl
{
	matrix_stack<float, 4> modelview;
	matrix_stack<float, 4> projection;
	matrix_stack<float, 4> texture;
	matrix_stack<float, 4> color;
};

#endif
