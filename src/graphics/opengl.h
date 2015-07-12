/*
 * opengl.h
 *
 *  Created on: Feb 16, 2014
 *      Author: nbingham
 */

#ifndef opengl_h
#define opengl_h

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <core/vector.h>

template <class type>
void gl_rotate(core::vector<type, 3> v)
{
	glRotated((double)v[0], 1.0, 0.0, 0.0);
	glRotated((double)v[1], 0.0, 1.0, 0.0);
	glRotated((double)v[2], 0.0, 0.0, 1.0);
}

template <class type>
void gl_translate(core::vector<type, 3> v)
{
	glTranslated((double)v[0], (double)v[1], (double)v[2]);
}


#endif
