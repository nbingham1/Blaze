/*
 * series.h
 *
 *  Created on: May 24, 2015
 *      Author: nbingham
 */

#pragma once

#include <math/geometry.h>
#include <std/array.h>
#include <type_traits>
#include <std/io.h>

using namespace core;

#define m_pi_2 1.570796327

template <class tc, class tw, int s>
struct spectra
{

	spectra()
	{
	}

	spectra(double first, ...)
	{
		va_list arguments;
		int i;

		va_start(arguments, first);
		this->c = (tc)first;
		for (i = 0; i < s; i++)
			this->w[i] = (tw)va_arg(arguments, double);
		this->p = (tw)va_arg(arguments, double);
		va_end(arguments);
	}

	spectra(float first, ...)
	{
		va_list arguments;
		int i;

		va_start(arguments, first);
		this->c = (tc)first;
		for (i = 0; i < s; i++)
			this->w[i] = (tw)va_arg(arguments, double);
		this->p = (tw)va_arg(arguments, double);
		va_end(arguments);
	}

	tc c;
	vector<tw, s> w;
	tw p;

	tw freq2() {
		return mag2(w);
	}

	tc operator()(vector<tw, s> v) {
		return c*cos(dot(w, v) + p);
	}

	template <int s2>
	spectra<tc, tw, s2> map(vector<vector<tw, s>, s2> v) {
		spectra<tc, tw, s2> result;
		result.c = c;
		result.p = p;
		for (int i = 0; i < s2; i++) {
			result.w[i] = dot(w, v[i]);
		}
		return result;
	}

	bool cant_merge(spectra<tc, tw, s> b) {
		bool wne, pne;
		if (std::is_same<tw, float>::value || std::is_same<tw, double>::value) {
			wne = mag2(this->w - b.w) > m_epsilon;
			pne = (this->p - b.p)*(this->p - b.p) > m_epsilon;
		} else {
			wne = this->w != b.w;
			pne = this->p != b.p;
		}

		return wne or (!std::is_same<tc, tw>::value and pne);
	}

	spectra<tc, tw, s> merge(spectra<tc, tw, s> b) {
		bool cne, pne;
		if (std::is_same<tw, float>::value || std::is_same<tw, double>::value) {
			pne = (this->p - b.p)*(this->p - b.p) > m_epsilon;
		} else {
			pne = this->p != b.p;
		}

		if (std::is_same<tc, float>::value || std::is_same<tc, double>::value) {
			cne = (this->c - b.c)*(this->c - b.c) > m_epsilon;
		} else {
			cne = this->c != b.c;
		}

		// assumes this->w == b.w
		if (pne and cne) {
			// assumes tc == tw

			// c0*cos(a*x + b0) + c1*cos(a*x + b1)
			// = c0*(cos(a*x)*cos(b0) - sin(a*x)*sin(b0)) + c1*(cos(a*x)*cos(b1) - sin(a*x)*sin(b1))
			// = (c0*cos(b0) + c1*cos(b1))*cos(a*x) - (c0*sin(b0) + c1*sin(b1))*sin(a*x)
			// (c0*cos(b0) + c1*cos(b1)) = c*cos(b)
			// -(c0*sin(b0) + c1*sin(b1)) = c*sin(b)
			// tan(b) = -(c0*sin(b0) + c1*sin(b1))/(c0*cos(b0) + c1*cos(b1))
			// b = atan2(-c0*sin(b0) - c1*sin(b1), c0*cos(b0) + c1*cos(b1))
			// c = sqrt((c0*cos(b0) + c1*cos(b1))^2 + (c0*sin(b0) + c1*sin(b1))^2)
			// = c*cos(a*x + b)
			tc sin_c = -this->c*sin(this->p) - b.c*sin(b.p);
			tc cos_c = this->c*cos(this->p) + b.c*cos(b.p);
			this->p = atan2(sin_c, cos_c);
			this->c = sqrt(sin_c*sin_c + cos_c*cos_c);
		} else if (pne) {
			// assumes tc == tw

			// cos(a*x + b0) + cos(a*x + b1) = 2*cos((b0-b1)/2)*cos(a*x+(b0+b1)/2)
			this->c *= (tc)2*cos((this->p - b.p)/(tc)2);
			this->p = (this->p + b.p)/(tc)2;
		} else {
			// c0*cos(a*x + b) + c1*cos(a*x + b) = (c0+c1)*cos(a*x + b)
			this->c += b.c;
		}
		return *this;
	}

	spectra<tc, tw, s> integral(int axis) {
		spectra<tc, tw, s> result = *this;
		result.p += (tw)m_pi_2;
		result.c /= result.w[axis];
		return result;
	}

	spectra<tc, tw, s> derivative(int axis) {
		spectra<tc, tw, s> result = *this;
		result.p -= (tw)m_pi_2;
		result.c *= result.w[axis];
		return result;
	}

	spectra<tc, tw, s> &integrate(int axis) {
		this->p += (tw)m_pi_2;
		this->c /= this->w[axis];
		return *this;
	}

	spectra<tc, tw, s> &differentiate(int axis) {
		this->p -= (tw)m_pi_2;
		this->c *= this->w[axis];
		return *this;
	}

	static spectra<tc, tw, s> constant(tc value) {
		spectra<tc, tw, s> result;
		result.p = (tw)0;
		for (int i = 0; i < s; i++) {
			result.w[i] = (tw)0;
		}
		result.c = value;
		return result;
	}
};

typedef spectra<float, float, 1> spec1f;
typedef spectra<float, float, 2> spec2f;
typedef spectra<float, float, 3> spec3f;

typedef spectra<double, double, 1> spec1d;
typedef spectra<double, double, 2> spec2d;
typedef spectra<double, double, 3> spec3d;

template <class tc, class tw, int s>
stream<string> &operator<<(stream<string> &f, spectra<tc, tw, s> v)
{
	f << v.c << "cos(" << v.w << "*v + " << v.p << ")";
	return f;
}

template <class tc, class tw, int s>
spectra<tc, tw, s> operator-(spectra<tc, tw, s> a) {
	a.c = -a.c;
	return a;
}

// executes the positive side of the multiply
template <class tc, class tw, int s>
spectra<tc, tw, s> operator+(spectra<tc, tw, s> a, spectra<tc, tw, s> b) {
	a.w += b.w;
	a.p += b.p;
	a.c *= b.c/(tc)2;
	return a;
}

// executes the negative side of the multiply
template <class tc, class tw, int s>
spectra<tc, tw, s> operator-(spectra<tc, tw, s> a, spectra<tc, tw, s> b) {
	a.w -= b.w;
	a.p -= b.p;
	a.c *= b.c/(tc)2;
	return a;
}

template <class tc, class tw, int s>
bool operator<(spectra<tc, tw, s> a, spectra<tc, tw, s> b) {
	tw fa = a.freq2();
	tw fb = b.freq2();
	if (fa < fb) {
		return true;
	} else if (fa > fb) {
		return false;
	}

	for (int i = 0; i < s; i++) {
		if (a.w[i] < b.w[i]) {
			return true;
		} else if (a.w[i] > b.w[i]) {
			return false;
		}
	}

	return false;
}

template <class tc, class tw, int s>
bool operator<=(spectra<tc, tw, s> a, spectra<tc, tw, s> b) {
	tw fa = a.freq2();
	tw fb = b.freq2();
	if (fa < fb) {
		return true;
	} else if (fa > fb) {
		return false;
	}

	for (int i = 0; i < s; i++) {
		if (a.w[i] < b.w[i]) {
			return true;
		} else if (a.w[i] > b.w[i]) {
			return false;
		}
	}

	return true;
}


template <class tc, class tw, int s>
bool operator==(spectra<tc, tw, s> a, spectra<tc, tw, s> b) {
	return a.w == b.w && a.p == b.p;
}

template <class tc, class tw, int s>
struct simple_series
{
	array<spectra<tc, tw, s> > elems;

	void push_back(spectra<tc, tw, s> a) {
		bool c0;
		if (std::is_same<tc, float>::value or std::is_same<tc, double>::value) {
			c0 = a.c*a.c < m_epsilon;
		} else {
			c0 = a.c == (tc)0;
		}

		if (c0) {
			return;
		}

		if (elems.size() == 0 or elems.back().cant_merge(a)) {
			elems.push_back(a);
		} else {
			elems.back().merge(a);
		}
	}

	void insert(spectra<tc, tw, s> a) {
		bool c0;
		if (std::is_same<tc, float>::value or std::is_same<tc, double>::value) {
			c0 = a.c*a.c < m_epsilon;
		} else {
			c0 = a.c == (tc)0;
		}

		if (c0) {
			return;
		}

		auto i = lower_bound(elems, a);
		if (!i or i->cant_merge(a)) {
			i.push(a);
		} else {
			i->merge(a);
		}
	}

	template <int s2>
	simple_series<tc, tw, s2> map(vector<vector<tw, s>, s2> v) {
		simple_series<tc, tw, s2> result;
		for (auto i = elems.begin(); i != elems.end(); i++) {
			result.push_back(i->map(v));
		}
		return result;
	}

	simple_series<tc, tw, s> integral(int axis) {
		simple_series<tc, tw, s> result;
		for (auto i = elems.begin(); i != elems.end(); i++) {
			if (i->w[axis] != (tw)0) {
				result.push_back(i->integral(axis));
			}
		}
		return result;
	}

	simple_series<tc, tw, s> derivative(int axis) {
		simple_series<tc, tw, s> result;
		for (auto i = elems.begin(); i != elems.end(); i++) {
			result.push_back(i->derivative(axis));
		}
		return result;
	}

	simple_series<tc, tw, s> &integrate(int axis) {
		for (auto i = elems.begin(); i != elems.end(); i++) {
			if (i->w[axis] != (tw)0) {
				i->integrate(axis);
			}
		}
		return *this;
	}

	simple_series<tc, tw, s> &differentiate(int axis) {
		for (auto i = elems.begin(); i != elems.end(); i++) {
			i->differentiate(axis);
		}
		return *this;
	}

	tc operator()(vector<tw, s> x)
	{
		tc result = (tc)0;
		for (auto i = elems.begin(); i != elems.end(); i++) {
			result += (*i)(x);
		}
		return result;
	}

	static simple_series<tc, tw, s> constant(tc value) {
		simple_series<tc, tw, s> result;
		result.elems.push_back(spectra<tc, tw, s>::constant(value));
		return result;
	}
};

template <class tc, class tw, int s>
stream<string> &operator<<(stream<string> &f, simple_series<tc, tw, s> v)
{
	f << v.elems.size();
	/*for (auto i = v.elems.begin(); i != v.elems.end(); i++) {
		if (i != v.elems.begin())
			f << " + ";
		f << *i;
	}*/
	return f;
}

template <class tc, class tw, int s>
simple_series<tc, tw, s> operator-(simple_series<tc, tw, s> a)
{
	simple_series<tc, tw, s> result;
	for (auto i = a.elems.begin(); i != a.elems.end(); i++) {
		result.elems.push_back(*i);
		result.elems.back().c = -result.elems.back().c;
	}
	return result;
}

template <class tc, class tw, int s>
simple_series<tc, tw, s> operator*(simple_series<tc, tw, s> a, simple_series<tc, tw, s> b) {
	simple_series<tc, tw, s> result;
	result.elems.reserve(a.elems.size()*b.elems.size()*2);
	for (auto i = a.elems.begin(); i != a.elems.end(); i++) {
		for (auto j = b.elems.begin(); j != b.elems.end(); j++) {
			result.insert(*i + *j);
			result.insert(*i - *j);
		}
	}
	return result;
}

template <class tc, class tw, int s>
simple_series<tc, tw, s> operator+(simple_series<tc, tw, s> a, simple_series<tc, tw, s> b) {
	simple_series<tc, tw, s> result;
	result.elems.reserve(a.elems.size() + b.elems.size());
	for (auto i = a.elems.begin(), j = b.elems.begin();
	     i != a.elems.end() || j != b.elems.end(); ) {
		if (j == b.elems.end() || i != a.elems.end() && *i < *j) {
			result.push_back(*i);
			i++;
		} else {
			result.push_back(*j);
			j++;
		}
	}
	return result;
}

template <class tc, class tw, int s>
simple_series<tc, tw, s> operator-(simple_series<tc, tw, s> a, simple_series<tc, tw, s> b) {
	simple_series<tc, tw, s> result;
	result.elems.reserve(a.elems.size() + b.elems.size());
	for (auto i = a.elems.begin(), j = b.elems.begin();
	     i != a.elems.end() || j != b.elems.end(); ) {
		if (j == b.elems.end() || i != a.elems.end() && *i < *j) {
			result.push_back(*i);
			i++;
		} else {
			result.push_back(-*j);
			j++;
		}
	}
	return result;
}

typedef simple_series<float, float, 1> sseries1f;
typedef simple_series<float, float, 2> sseries2f;
typedef simple_series<float, float, 3> sseries3f;

template <class tc, class tw, int s>
struct series
{
	simple_series<tc, tw, s> numerator;
	simple_series<tc, tw, s> denominator;

	series<tc, tw, s> derivative(int axis) {
		// d/dx f(x)/g(x) = (g(x)*f'(x) - f(x)*g'(x))/g(x)^2
		series<tc, tw, s> result;
		result.numerator = numerator.derivative(axis)*denominator - numerator*denominator.derivative(axis);
		result.denominator = denominator*denominator;
		return result;
	}

	series<tc, tw, s> &differentiate(int axis) {
		// d/dx f(x)/g(x) = (g(x)*f'(x) - f(x)*g'(x))/g(x)^2
		numerator = numerator.derivative(axis)*denominator - numerator*denominator.derivative(axis);
		denominator = denominator*denominator;
		return *this;
	}

	series<tc, tw, s> next(simple_series<tc, tw, s> f1, int axis)
	{
		// Ti = -Ti-1'/f'(x)
		// d/dx f(x)/g(x) = (g(x)*f'(x) - f(x)*g'(x))/g(x)^2

		series<tc, tw, s> result;
		result.numerator = numerator*denominator.derivative(axis) - numerator.derivative(axis)*denominator;
		result.denominator = denominator*denominator*f1;
		return result;
	}

	tc operator()(vector<tw, s> x)
	{
		return numerator(x)/denominator(x);
	}
};

typedef series<float, float, 1> series1f;
typedef series<float, float, 2> series2f;
typedef series<float, float, 3> series3f;

template <class tc, class tw, int s>
stream<string> &operator<<(stream<string> &f, series<tc, tw, s> v)
{
	f << "(" << v.numerator << ")/(" << v.denominator << ")";
	return f;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator-(series<tc, tw, s> a)
{
	series<tc, tw, s> result;
	result.numerator = -a.numerator;
	result.denominator = a.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator+(series<tc, tw, s> a, series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator*b.denominator + b.numerator*a.denominator;
	result.denominator = a.denominator*b.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator-(series<tc, tw, s> a, series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator*b.denominator - b.numerator*a.denominator;
	result.denominator = a.denominator*b.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator*(series<tc, tw, s> a, series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator*b.numerator;
	result.denominator = a.denominator*b.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator/(series<tc, tw, s> a, series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator*b.denominator;
	result.denominator = a.denominator*b.numerator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator+(series<tc, tw, s> a, simple_series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator + b*a.denominator;
	result.denominator = a.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator-(series<tc, tw, s> a, simple_series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator - b*a.denominator;
	result.denominator = a.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator*(series<tc, tw, s> a, simple_series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator*b;
	result.denominator = a.denominator;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> operator/(series<tc, tw, s> a, simple_series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = a.numerator;
	result.denominator = a.denominator*b;
	return result;
}

template <class t2, class tc, class tw, int s>
series<tc, tw, s> operator/(t2 a, simple_series<tc, tw, s> b)
{
	series<tc, tw, s> result;
	result.numerator = simple_series<tc, tw, s>::constant((tc)a);
	result.denominator = b;
	return result;
}

template <class tc, class tw, int s>
series<tc, tw, s> project(simple_series<tc, tw, s> color, simple_series<tc, tw, s> alpha, int axis, int count)
{
	// This operation should be pre-cached and then just mapped to the projection
	// space, then evaluated in the shaders.

	//   e^-color^2 * (1-e^-alpha^2) * e^int(ln(1-(1-e^-alpha^2)))
	// = e^-color^2 * (1-e^-alpha^2) * e^-int(alpha^2)
	// = e^(-color^2 - int(alpha^2)) - e^(-color^2 - alpha^2 - int(alpha^2))
	// TODO filter mapped series for boundaries and resolution
	
	simple_series<tc, tw, s> c2 = color * color;
	cout << "c2 = " << c2 << endl;

	simple_series<tc, tw, s> a2 = alpha * alpha;
	cout << "a2 = " << a2 << endl;
	simple_series<tc, tw, s> A2 = a2.integral(axis);
	cout << "A2 = " << A2 << endl;
	simple_series<tc, tw, s> p0 = -c2 - A2;
	cout << "p0 = " << p0 << endl;
	simple_series<tc, tw, s> p1 = p0 - a2;
	cout << "p1 = " << p1 << endl;
	
	simple_series<tc, tw, s> dp0 = p0.derivative(axis);
	cout << "dp0 = " << dp0 << endl;
	simple_series<tc, tw, s> dp1 = p1.derivative(axis);
	cout << "dp1 = " << dp1 << endl;

	series<tc, tw, s> t0 = 1/dp0;
	cout << "t0 = " << t0 << endl;
	series<tc, tw, s> t1 = 1/dp1;
	cout << "t1 = " << t1 << endl;

	series<tc, tw, s> result;
	for (int i = 0; i < count; i++) {
		result = result+t0+t1;
		t0 = t0.next(dp0, axis);
		cout << "t0 = " << t0 << endl;
		t1 = t1.next(dp1, axis);
		cout << "t1 = " << t1 << endl;
	}
	return result;
}

