#include "shader.h"
#include <stdio.h>

static long GetFileLength( FILE *pFile )
{
	long nSaved = ftell( pFile ), nSize;
	fseek( pFile, 0, SEEK_END );
	nSize = ftell( pFile );
	fseek( pFile, nSaved, SEEK_SET );
	return nSize;
}

GLhandleARB LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength)
{
	GLhandleARB Handle = 0;

	FILE *InFile = fopen(SourceFile, "rt");
	if (InFile == NULL)
		return false;

	GLint Length = (GLint)GetFileLength(InFile);
	GLcharARB *Shader = new char[Length + 1];

	if (Length == 0 || Shader == NULL)
		return NULL;

	Length = (GLint)fread(Shader, sizeof(GLcharARB), Length, InFile);
	Shader[Length] = '\0';
	fclose(InFile);

	Handle = glCreateShaderObjectARB(ShadeType);
	glShaderSourceARB(Handle, 1, (const GLcharARB**)&Shader, &Length);
	glCompileShaderARB(Handle);
	delete [] Shader;

	if (ErrorLog && ErrorLength)
	{
		GLint TotalLength = 0, nCharsWritten = 0;

		glGetObjectParameterivARB(Handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &TotalLength);
		if(TotalLength > 1)
			glGetInfoLogARB(Handle, *ErrorLength, &nCharsWritten, ErrorLog);

		*ErrorLength = TotalLength - 1;
	}

	return Handle;
}
