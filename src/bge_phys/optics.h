#include "../bge_math.h"
#include "../bge_mtrl.h"

#ifndef optics_h
#define optics_h

rgba_hdr scatter(double view_angle, double view_height, double source_angle, rgb_hdr source_color);

#endif
