#include "../common.h"
#include "color.h"

#ifndef shader_h
#define shader_h

GLuint LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength);
GLuint LoadGLSLShaderSource(GLenum ShadeType, char *Source, char *ErrorLog, int *ErrorLength);
GLuint LoadGLSLShaderDynamic(GLenum ShadeType, const char *SourceFile, char *Variables, char *Functions, char *Data, char *ErrorLog, int *ErrorLength);

#endif
