#include "standard.h"
#include "mathematics.h"
#include "graphics.h"

#ifndef texture_h
#define texture_h

unsigned char *rgb_tga(char *filename, int *w, int *h);
unsigned char *rgba_tga(char *rgb, char *a, int *w, int *h);
unsigned char *rgba_tga(char *rgb, int *w, int *h);

#endif
