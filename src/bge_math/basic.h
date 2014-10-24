/*
 * basic.h
 *
 *  Created on: Aug 18, 2011
 *      Author: Ned Bingham
 */

#ifndef basic_h
#define basic_h

#define m_pi 3.14159265358979323
#define m_e 2.718281828
#define m_epsilon 0.000001

#define m_sqr(n) (n*n)
#define m_cube(n) (n*n*n)

#define m_abs(n) ((n) < 0.0 ? -(n) : (n))
#define m_min(m, n) (m < n ? m : n)
#define m_max(m, n) (m < n ? n : m)

#define m_nan(n) (!(n > 0 || n <= 0))

#define m_clamp(n, a, b) m_min(m_max(n, a), b)

#define degtorad(a) m_pi*a/180.0
#define radtodeg(a) 180.0*a/m_pi

#define lerp(t, a, b) (a*(1.0 - t) + b*t)

#define m_mod(m, n) (m - floor(m/n)*n)

#endif
