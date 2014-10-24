/*
 * common.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on August 18, 2011.
 * Modified by Ned Bingham on August 18, 2011.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef common_h
#define common_h

#include "cmn/generic.h"
#include "cmn/lock.h"
#include "cmn/message.h"

#define SET_FLAG(res, flag)		(res |= flag)
#define UNSET_FLAG(res, flag)	(res &= ~flag)
#define TOGGLE_FLAG(res, flag)	(res ^= flag)
#define GET_FLAG(res, flag) 	((res & flag) > 0)
#define FLAG_NUMBER(n)			(1 << (n))

#define EXPAND1(arr, stride)   (*(arr))
#define EXPAND10(arr, stride)  (*(arr)),(*(arr + 1*stride)),(*(arr + 2*stride)),(*(arr + 3*stride)),(*(arr + 4*stride)),(*(arr + 5*stride)),(*(arr + 6*stride)),(*(arr + 7*stride)),(*(arr + 8*stride)),(*(arr + 9*stride))
#define EXPAND100(arr, stride) EXPAND10(arr, stride),EXPAND10(arr+10*stride, stride),EXPAND10(arr+20*stride, stride),EXPAND10(arr+30*stride, stride),EXPAND10(arr+40*stride, stride),EXPAND10(arr+50*stride, stride),EXPAND10(arr+60*stride, stride),EXPAND10(arr+70*stride, stride),EXPAND10(arr+80*stride, stride),EXPAND90(arr+10*stride, stride)

#include <cstdarg>

#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

using namespace std;

#endif
