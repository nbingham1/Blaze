#include "optics.h"

rgba_hdr scatter(double view_angle, double view_height, double source_angle, rgb_hdr source_color)
{
	float refraction_index = 1.0;
	float air_density = 1.0;
	float K = 2*m_sqr(m_pi)*m_sqr(m_sqr(refraction_index) - 1.0)/(3.0*air_density);
	float Fr = 0.75*(1.0 + m_sqr(cos(source_angle)));
	//float Ho = 1200.0;
	//float h = view_height;
	//float p = exp(-h/Ho);
	rgb_hdr ret = source_color*K*Fr/rgb_hdr(2.401e-25, 9.150625e-26, 4.100625e-26);
	return rgba_hdr(ret.r, ret.g, ret.b, 1.0);
}
