#include "vertex.h"

vertex_v3n3t3g3 &vertex_v3n3t3g3::operator=(vertex_v3n3t3g3 v)
{
	this->v[0] = v.v[0];
	this->v[1] = v.v[1];
	this->v[2] = v.v[2];
	this->n[0] = v.n[0];
	this->n[1] = v.n[1];
	this->n[2] = v.n[2];
	this->t[0] = v.t[0];
	this->t[1] = v.t[1];
	this->t[2] = v.t[2];
	this->g[0] = v.g[0];
	this->g[1] = v.g[1];
	this->dn[0] = v.dn[0];
	this->dn[1] = v.dn[1];
	this->dn[2] = v.dn[2];
	return *this;
}

void vertex_v3n3t3g3::copywithshift(vertex_v3n3t3g3 v, float *voff)
{
	this->v[0] = v.v[0] + voff[0];
	this->v[1] = v.v[1] + voff[1];
	this->v[2] = v.v[2] + voff[2];
	this->n[0] = v.n[0];
	this->n[1] = v.n[1];
	this->n[2] = v.n[2];
	this->t[0] = v.t[0];
	this->t[1] = v.t[1];
	this->t[2] = v.t[2];
	this->g[0] = v.g[0];
	this->g[1] = v.g[1];
	this->dn[0] = v.dn[0];
	this->dn[1] = v.dn[1];
	this->dn[2] = v.dn[2];
}
