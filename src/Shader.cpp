#include "standard.h"
#include "shader.h"

GLhandleARB LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength)
{
	GLhandleARB Handle = 0;

	FILE *InFile = fopen(SourceFile, "rt");
	if (InFile == NULL)
		return false;

	GLint Length = (GLint)fsize(InFile);
	GLcharARB *Shader = new char[Length + 1];

	if (Length == 0 || Shader == NULL)
		return NULL;

	Length = (GLint)fread(Shader, sizeof(GLcharARB), Length, InFile);
	Shader[Length] = '\0';
	fclose(InFile);
	Handle = glCreateShaderObjectARB(ShadeType);

	printf("Handle: %d\n", Handle);

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

GLhandleARB LoadGLSLShaderSource(GLenum ShadeType, GLcharARB *Source, char *ErrorLog, int *ErrorLength)
{
	GLhandleARB Handle = 0;

	GLint Length = sizeof(Source)/sizeof(GLcharARB);

	if (Length == 0 || Source == NULL)
		return NULL;

	Handle = glCreateShaderObjectARB(ShadeType);
	glShaderSourceARB(Handle, 1, (const GLcharARB**)&Source, &Length);
	glCompileShaderARB(Handle);

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
