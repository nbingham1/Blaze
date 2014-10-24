#ifndef shader_h
#define shader_h

#include "graphics.h"

struct rgba
{
	float r;
	float g;
	float b;
	float a;
};

struct rgb
{
	float r;
	float g;
	float b;
};

GLhandleARB LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength);
GLhandleARB LoadGLSLShaderSource(GLenum ShadeType, char *Source, char *ErrorLog, int *ErrorLength);

#endif
