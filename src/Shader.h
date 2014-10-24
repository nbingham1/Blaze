#include "OpenGLIncludes.h"

#ifndef Shader_h
#define Shader_h

GLhandleARB LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength);
GLhandleARB LoadGLSLShaderSource(GLenum ShadeType, char *Source, char *ErrorLog, int *ErrorLength);

#endif