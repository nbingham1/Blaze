#ifndef shader_h
#define shader_h

#include "graphics.h"

struct rgba
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct rgb
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

GLhandleARB LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength);
GLhandleARB LoadGLSLShaderSource(GLenum ShadeType, char *Source, char *ErrorLog, int *ErrorLength);

#endif
