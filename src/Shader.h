#include "graphics.h"

#ifndef Shader_h
#define Shader_h

GLhandleARB LoadGLSLShader(GLhandleARB ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength);
GLhandleARB LoadGLSLShaderSource(GLhandleARB ShadeType, char *Source, char *ErrorLog, int *ErrorLength);

#endif
