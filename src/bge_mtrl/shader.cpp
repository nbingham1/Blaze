#include "shader.h"
#include "../bge_input.h"

GLuint LoadGLSLShader(GLenum ShadeType, const char *SourceFile, char *ErrorLog, int *ErrorLength)
{
	GLuint Handle = 0;

	FILE *InFile = fopen(SourceFile, "rt");
	if (InFile == NULL)
	{
		sprintf(ErrorLog, "Error: Could not find file: %s.\n", SourceFile);
		return 0;
	}

	GLint Length = (GLint)fsize(InFile);
	char *Shader = new char[Length + 1];

	if (Length == 0 || Shader == NULL)
	{
		sprintf(ErrorLog, "Error: Could not allocate the space.\n");
		return 0;
	}

	Length = (GLint)fread(Shader, sizeof(char), Length, InFile);
	Shader[Length] = '\0';

	fclose(InFile);
	Handle = glCreateShader(ShadeType);

	glShaderSource(Handle, 1, (const char**)&Shader, &Length);
	glCompileShader(Handle);
	delete [] Shader;

	if (ErrorLog && ErrorLength)
	{
		GLint TotalLength = 0, nCharsWritten = 0;

		glGetShaderInfoLog(Handle, *ErrorLength, &nCharsWritten, ErrorLog);

		*ErrorLength = TotalLength - 1;
	}

	return Handle;
}

GLuint LoadGLSLShaderSource(GLenum ShadeType, char *Source, char *ErrorLog, int *ErrorLength)
{
	GLuint Handle = 0;

	GLint Length = sizeof(Source)/sizeof(char);

	if (Length == 0 || Source == NULL)
		return 0;

	Handle = glCreateShader(ShadeType);
	glShaderSource(Handle, 1, (const char**)&Source, &Length);
	glCompileShader(Handle);

	GLint nCharsWritten = 0;
	if (ErrorLog && ErrorLength)
		glGetShaderInfoLog(Handle, *ErrorLength, &nCharsWritten, ErrorLog);

	return Handle;
}

GLuint LoadGLSLShaderDynamic(GLenum ShadeType, const char *SourceFile, char *Variables, char *Functions, char *Data, char *ErrorLog, int *ErrorLength)
{
	GLuint Handle = 0;

	FILE *InFile = fopen(SourceFile, "rt");
	if (InFile == NULL)
	{
		sprintf(ErrorLog, "Error: Could not find file: %s.\n", SourceFile);
		return 0;
	}

	GLint Length = (GLint)fsize(InFile);
	int VariableLength = strlen(Variables)+1;
	int FunctionLength = strlen(Functions)+1;
	int DataLength = strlen(Data)+1;

	if (Length == 0)
	{
		sprintf(ErrorLog, "Error: Nothing in the file.\n");
		return 0;
	}

	char *Shader = new char[Length + 1 + VariableLength + FunctionLength + DataLength];
	Shader[0] = '\0';
	char line[256];
	while (!feof(InFile))
	{
		fgets(line, 256, InFile);

		if (strncmp(line, "// variables", 12) == 0)
		{
			strcat(Shader, line);
			strcat(Shader, Variables);
		}

		else if (strncmp(line, "// functions", 12) == 0)
		{
			strcat(Shader, line);
			strcat(Shader, Functions);
		}

		else if (strncmp(line, "	// data", 8) == 0)
		{
			strcat(Shader, line);
			strcat(Shader, Data);
		}

		else
			strcat(Shader, line);


	}
	Length = Length + VariableLength + FunctionLength + DataLength;

	fclose(InFile);
	Handle = glCreateShader(ShadeType);

	glShaderSource(Handle, 1, (const char**)&Shader, &Length);
	glCompileShader(Handle);
	delete [] Shader;

	if (ErrorLog && ErrorLength)
	{
		GLint TotalLength = 0, nCharsWritten = 0;

		glGetShaderInfoLog(Handle, *ErrorLength, &nCharsWritten, ErrorLog);

		*ErrorLength = TotalLength - 1;
	}

	return Handle;
}
