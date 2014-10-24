/*
 * basic.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on August 18, 2011.
 * Modified by Ned Bingham on June 9, 2012.
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

#ifndef basic_h
#define basic_h

#include <math.h>

#define m_pi 3.14159265358979323
#define m_e 2.718281828
#define m_epsilon 0.000001

#define degtorad(a) m_pi*a/180.0
#define radtodeg(a) 180.0*a/m_pi

#define powc(b, e) ((e&1)>0?b:1)*((e&2)>0?b*b:1)*((e&4)>0?b*b*b*b:1)

template <class t>
t fast_pow(t b, int e)
{
	t i, r = 1;

	for (i = b; e > 0; i *= i, e >>= 1)
		r = (e & 0x1) > 0 ? r*i : r;

	return r;
}

template <class type>
int sgn(type val)
{
    return (type(0) < val) - (val < type(0));
}

template <class type>
const type &minimum(const type &a, const type &b)
{
	return a < b ? a : b;
}

template <class type>
const type &maximum(const type &a, const type &b)
{
	return a > b ? a : b;
}

template <class type>
type absolute(const type &a)
{
	return a < 0 ? -a : a;
}

template <class type>
const type &clamp(const type &a, const type &b, const type &c)
{
	return minimum(maximum(a, b), c);
}


bool is_even(int i);
bool is_odd(int i);
int log2i(int i);

#endif
